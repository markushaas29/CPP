#include "Element.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.h"
#include "../Validator/Validator.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <string.h>
#include <map>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#pragma once
//~ #ifndef ELEMENTS_HPP
//~ #define ELEMENTS_HPP

//--------------------------------TYPES------------------------------------------------
template<typename T = std::string>
class Key: public Element
{
public:
	inline static const std::string Identifier = "Key";
	Key(std::string s = ""): Element(s.c_str()) {};
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
inline bool operator< (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() < rhs.Value(); }

template<typename T = std::string>
inline bool operator== (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() == rhs.Value(); }

class IBAN: public Element
{
	static constexpr bool isValid(const char* iban)
	{
		if (!SizeValidator<Length>::Condition(iban) || !LetterValidator::Condition(iban[0]) || !LetterValidator::Condition(iban[1]))
			return false;

		for(int i = 2; i < Length; ++i)
			if(!NumberValidator::Condition(iban[i]))
				return false;
			
		return true;
	}
	static constexpr const char* check(const char* iban) { return isValid(iban) ? iban : Default; }
public:
	inline static constexpr uint Length = 22;
	inline static constexpr const char* Default = "XX00000000000000000000";
	inline static const std::string Identifier = "IBAN";
	IBAN(std::string s): IBAN{s.c_str()}
	{
		if(!isValid(s.c_str()))
			Logger::Log<Error>("IBAN",s," is invalid!");
	};
	constexpr IBAN(const char* c): Element(check(c)){	};
	constexpr IBAN(): Element(""){ };
	IBAN* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
};

inline bool operator< (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() < rhs.Value(); }
inline bool operator== (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() == rhs.Value(); }

class BIC: public Element
{
public:
	inline static const std::string Identifier = "BIC";
	BIC(std::string s): BIC(s.c_str()){};
	constexpr BIC(const char* c): Element(c){ };
	constexpr BIC(): Element(""){ };
	BIC* DoCreate(){return this;};
};


template<typename T = std::string>
class Item: public Element
{
public:
	inline static const std::string Identifier = "Item";
	Key<T> key;
	Item(std::string s):Element(s.c_str()), key(s){};
	constexpr Item(const char* c): Element(c){ };
	Item* DoCreate(){return this;};
};

class Entry: public Element
{
public:
	inline static const std::string Identifier = "Entry";
    Entry(std::string s): Entry(s.c_str()){};
	constexpr Entry(const char* c): Element(c){ };
    constexpr Entry(): Element(""){};
    Entry* DoCreate(){return this;};
};

class Name: public Element
{
public:
    inline static const std::string Identifier = "Name";
    Name(std::string s): Name(s.c_str()){};
	constexpr Name(const char* c): Element(c){ };
    constexpr Name(): Element(""){};
    Name* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
};

template<typename D, typename U, typename T = double>
class Value: public Element
{
public:
	using Derived = D;
	using Unit = U;
	using TQuantity = Quantity<Unit>;
	Value(std::string s = "0.0"): Element(s.c_str()), quantity(this->to(s)) {};
	Value(T t): Element(std::to_string(t).c_str()), quantity(t) {};
	Value(Quantity<U> u): Element(std::to_string(u.Value()).c_str()), quantity(u) {};
	const Quantity<U>& GetQuantity() const { return this->quantity; }
	const T& Get() { return this->val; }
	static const char* Key;
	Element* DoCreate() { return this; };
	
	Value& operator=(const Value& a)
	{
		this->quantity = a.quantity; 
		this->val = a.val; 
		return *this; 
	}
	
	decltype(auto) ID() { return Key; }
private:
	Quantity<U> quantity;
	T val;
	String_::To<T> to;
};

template<typename D, typename U, typename T = double>
std::ostream& operator<<(std::ostream& out, const Value<D,U,T>& c) { return out<<D::Key<<"\t"<<c.GetQuantity(); }

class Date;

//~ #endif
