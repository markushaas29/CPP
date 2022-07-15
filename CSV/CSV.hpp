#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <memory>
#include <chrono>
#include <ctime>

#ifndef CSV_H
#define CSV_H

//--------------------------------Element------------------------------------------------
class Element
{
public:
	inline static const std::string Identifier = "";
	virtual Element* DoCreate() = 0;
	virtual ~Element(){}
	
	Element(std::string s):stringValue(s), Value(s) {};
	const std::string Value;	
private:
	const std::string stringValue;
};

std::ostream& operator<<(std::ostream& out, const Element& e)
{
	return out<<e.Value;
}

//--------------------------------CreateElementNewPolicy------------------------------------------------

template<class ConcreteProduct, typename T = std::string>
class CreateElementNewPolicy
{
public:
	static Element* DoCreate(T param)
	{
		return new ConcreteProduct(param);
	}
};


//--------------------------------TYPES------------------------------------------------
template<typename T = std::string>
class Key: public Element
{
public:
	inline static const std::string Identifier = "Key";
	Key(std::string s = ""): Element(s) {};
	Key* DoCreate(){return this;};
	bool operator==(std::string s) const{ return s == this->Value; }
	bool operator!=(std::string s) const{ return s != this->Value; }
	using ValueType = T;
	using Type = Key<T>;
	using ContainerType  = std::vector<T>;
	using Iterator  = std::vector<T>::const_iterator;
	using ContainerPtrType  = std::shared_ptr<ContainerType>;

	const ContainerPtrType Patterns() const { return this->patterns; }
	bool Matches(const std::string& k)
	{ 
		auto comparer = keyCompare(k);
		return std::find_if(this->patterns->cbegin(), this->patterns->cend(), comparer) != this->patterns->cend(); 
	}
	void UpdatePatterns(Iterator begin, Iterator end) { this->patterns =  std::make_shared<ContainerType>(begin, end);}
	ValueType Current() const { return this->currentPattern; }
	void setCurrent(ValueType v) { this->currentPattern = v; }
private:
	class keyCompare
	{
		const std::string key;
	public:
		keyCompare(const std::string& k): key{k}{}
		bool operator ()(const T& t) { return t == key; }
	};
	
	ValueType currentPattern;
	ContainerPtrType patterns = std::make_shared<ContainerType>();
};

template<typename T = std::string>
bool operator==(std::string s, const Key<T>& k) { return k == s; }

template<typename T = std::string>
bool operator!=(std::string s, const Key<T>& k) { return k != s; }

template<typename T = std::string>
inline bool operator< (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value < rhs.Value; }

template<typename T = std::string>
inline bool operator== (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value == rhs.Value; }

template<typename T = double>
class Value: public Element
{
	using ConverterT = String_::From<T>;
	ConverterT converter = ConverterT();
	
	T value;
	T Parse(std::string s){ return std::stod(s); }
public:
	using Type = T;
	
	inline static const std::string Identifier = "Value";
	Value(std::string s = ""): Element(s), value(this->Parse(s)){};
	Value(T val, std::string s = ""): Element(this->converter(val)), value(val){};
	Value* DoCreate(){return this;};
	T Get(){ return this->value; }
};

class IBAN: public Element
{
public:
	inline static const std::string Identifier = "IBAN";
	IBAN(std::string s): Element(s){ };
	IBAN(): Element(""){ };
	IBAN* DoCreate(){return this;};
};

class BIC: public Element
{
public:
	inline static const std::string Identifier = "BIC";
	BIC(std::string s): Element(s){};
	BIC(): Element(""){ };
	BIC* DoCreate(){return this;};
};


template<typename T = std::string>
class Item: public Element
{
public:
	inline static const std::string Identifier = "Item";
	Key<T> key;
	Item(std::string s): Element(s), key(s){};
	Item* DoCreate(){return this;};
};

class Entry: public Element
{
public:
       inline static const std::string Identifier = "Entry";
       Entry(std::string s): Element(s){};
       Entry(): Element(""){};
       Entry* DoCreate(){return this;};
};

class Name: public Element
{
public:
       inline static const std::string Identifier = "Name";
       Name(std::string s): Element(s){};
       Name(): Element(""){};
       Name* DoCreate(){return this;};
};

template<typename D, typename U, typename T = double>
class CSVValue: public Element
{
	using Derived = D;
public:
	using Unit = U;
	using TQuantity = Quantity<Unit>;
	CSVValue(std::string s = "0.0"): Element(s), quantity(this->to(s)) {};
	CSVValue(T t): Element(std::to_string(t)), quantity(t) {};
	CSVValue(Quantity<U> u): Element(std::to_string(u.Value())), quantity(u) {};
	const Quantity<U>& GetQuantity() const { return this->quantity; }
	const T& Value() { return this->val; }
	static const char* Key;
	Element* DoCreate() { return this; };
	
	CSVValue& operator=(const CSVValue& a)
	{
		this->quantity = a.quantity; 
		this->val = a.val; 
		return *this; 
	}
private:
	Quantity<U> quantity;
	T val;
	String_::To<T> to;
};

//Buchungstag;Valuta;Textschlüssel;Primanota;Zahlungsempfänger;ZahlungsempfängerKto;ZahlungsempfängerIBAN;ZahlungsempfängerBLZ;ZahlungsempfängerBIC;Vorgang/Verwendungszweck;Kundenreferenz;Währung;Umsatz;Soll/Haben


//--------------------------------Factory------------------------------------------------
class Date;

using Elements = boost::mpl::vector<Key<std::string>, Value<double>, Entry, Date>;

template<class TList = Elements, class Unit = Element,typename T = std::string,class IdentifierType = std::string, template<class> class CreatePolicy = CreateElementNewPolicy>
class ElementFactory
{
	using ProductList = TList;
	using Creator = Unit* (*)(T);
	
private:
	using AssocMap = std::map<IdentifierType,Creator>;
	inline static AssocMap associations_ = std::map<IdentifierType,Creator>();
	
	struct Register
	{
		template<class Type>
		void operator()(Type) const
		{
			RegisterImpl(Type::Identifier, CreatePolicy<Type>::DoCreate);
		};
	};
	
	static bool RegisterImpl(const IdentifierType& id, const Creator& creator)
	{
		return associations_.insert(std::make_pair(id,creator)).second;
	}
	
public:
	ElementFactory()
	{		
		boost::mpl::for_each<ProductList>(Register());
	}
	
	
	bool Unregister(const IdentifierType& id)
	{
		return associations_.erase(id) == 1;
	}
		
	Unit* Create(const IdentifierType& id,T param)
	{
		typename AssocMap::const_iterator i = associations_.find(id);
		
		if(i != associations_.end())
		{
			return (i->second)(param);
		}
		
		return (associations_.find(id)->second)(param);
	}
};

#endif
