#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <string.h>
#include <map>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#ifndef CSV_H
#define CSV_H


template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>   to_array_impl(T (&a)[N], std::index_sequence<I...>){  return { {a[I]...} };}
 
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]){  return to_array_impl(a, std::make_index_sequence<N>{}); }

namespace detail
{
    template<unsigned... digits>
    struct to_chars { static const char value[]; };

    template<unsigned... digits>
    constexpr char to_chars<digits...>::value[] = {('0' + digits)..., 0};

    template<unsigned rem, unsigned... digits>
    struct explode : explode<rem / 10, rem % 10, digits...> {};

    template<unsigned... digits>
    struct explode<0, digits...> : to_chars<digits...> {};
}

template<unsigned num>
struct num_to_string : detail::explode<num> {};

template<uint S>
//~ constexpr decltype(auto) convertIntegerToChar(uint N)
std::array<char,S> convertIntegerToChar(uint N)
{
    uint m = N;

    uint digit = 0;
    while (m) {
        digit++;
        m /= 10;
    }

    std::array<char,S> arr = {};
	std::array<char,S> arr1 = {};
 
    int index = 0;
    while (N) {
        arr1[++index] = N % 10 + '0';
        N /= 10;
    }
    
    
    std::cout<<index<<std::endl;

    for (uint i= 0; i < S; i++) {
        arr[i] = arr1[S - i -1];
		if(!(int)arr[i])
			arr[S-1] = '0';
    }
    
    std::cout<<arr1[3]<<std::endl;
    std::cout<<arr[1]<<std::endl;
    std::cout<<arr[2]<<std::endl;
    std::cout<<arr[3]<<std::endl;
 
    std::cout<<arr[0]<<std::endl;
    std::cout<<arr[S-1]<<std::endl;
 
    return arr;
}

constexpr char NumToChar(uint i){ return (char)(i + '0'); }

//--------------------------------Element------------------------------------------------
class Element
{
public:
	inline static constexpr size_t N = 512;
	inline static const std::string Identifier = "Element";
	constexpr Element(std::array<char,N> s): size{N}, data{s} { };
	constexpr Element(const char* s): size{constLen(s)}, data{constInit(s)} { };
	const std::string Value() const  { return std::string(data.data()); };	
private:
	std::size_t size;
	constexpr std::size_t constLen(const char * a)
	{
		const char* end = a;
		while(*end) ++end;
		return end - a;
	}
	
	constexpr std::array<char,N> constInit(const char * a)
	{
		std::array<char,N> result = {};
		int i = 0;
		const char* end = a;
		while(*end) 
		{
			result[i++] = *end;
			++end;
		}
		return result;
	}
	
	std::array<char,N> data;
};

std::ostream& operator<<(std::ostream& out, const Element& e) {	return out<<e.Value();}

//--------------------------------CreateElementNewPolicy------------------------------------------------

template<class ConcreteProduct, typename T = std::string>
class CreateElementNewPolicy
{
public:
	static Element* DoCreate(T param){	return new ConcreteProduct(param);	}
};


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
	Value(std::string s = ""): Element(s.c_str()), value(this->Parse(s)){};
	Value(T val, std::string s = ""): Element(this->converter(val)), value(val){};
	Value* DoCreate(){return this;};
	T Get(){ return this->value; }
};

class IBAN: public Element
{
public:
	inline static const std::string Identifier = "IBAN";
	IBAN(std::string s): IBAN{s.c_str()}{ };
	constexpr IBAN(const char* c): Element(c){ };
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
class CSVValue: public Element
{
public:
	using Derived = D;
	using Unit = U;
	using TQuantity = Quantity<Unit>;
	CSVValue(std::string s = "0.0"): Element(s.c_str()), quantity(this->to(s)) {};
	CSVValue(T t): Element(std::to_string(t).c_str()), quantity(t) {};
	CSVValue(Quantity<U> u): Element(std::to_string(u.Value()).c_str()), quantity(u) {};
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
	
	decltype(auto) ID() { return Key; }
private:
	Quantity<U> quantity;
	T val;
	String_::To<T> to;
};

template<typename D, typename U, typename T = double>
std::ostream& operator<<(std::ostream& out, const CSVValue<D,U,T>& c) { return out<<D::Key<<"\t"<<c.GetQuantity(); }

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
