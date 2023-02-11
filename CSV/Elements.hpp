#include "Element.hpp"
#include "../String/String_.hpp"
#include "../String/StringParser.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
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
class Key: public Element<Key<T>>
{
	using Base = Element<Key<T>>;
	friend class Element<Key<T>>; 
public:
	using ValueType = T;
	using Type = Key<T>;
	inline static constexpr const char* Identifier = "Key";
	Key(std::string s = ""): Base(s.c_str()) {};
	constexpr Key(const Key& k): Base(k.Value().c_str()){};
	constexpr Key(const char* k): Base(k){};

	bool operator==(T s) const{ return s == this->Value(); }
	bool operator<=>(T s) const{ return s <=> this->Value(); }
private:
	static constexpr const char* check(const char* s) { return s; }
};

template<typename T = std::string>
inline decltype(auto) operator<=> (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() <=> rhs.Value(); }

template<typename T = std::string>
inline bool operator== (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() == rhs.Value(); }


class IBAN: public Element<IBAN>
{
	using Base = Element<IBAN>;
	friend class Element<IBAN>;
public:
	inline static constexpr uint Length = 22;
	inline static constexpr const char* Default = "XX00000000000000000000";
	inline static constexpr const char* Identifier = "IBAN";
	IBAN(std::string s): IBAN{s.c_str()}
	{
		if(!isValid(s.c_str()))
			Logger::Log<Error>("IBAN",s," is invalid!");
	};
	constexpr IBAN(const char* c): Element(c){	};
	constexpr IBAN(): Element(""){ };
	IBAN* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
private:
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
};

inline bool operator< (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() < rhs.Value(); }
inline bool operator== (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() == rhs.Value(); }

class BIC: public Element<BIC>
{
	using Base = Element<BIC>;
	friend class Element<BIC>;
public:
	inline static constexpr const char* Identifier = "BIC";
	BIC(std::string s): BIC(s.c_str()){};
	constexpr BIC(const char* c): Element(c){ };
	constexpr BIC(): Element(""){ };
	BIC* DoCreate(){return this;};
private:
	static constexpr const char* check(const char* s) { return s; }
};


template<typename T = std::string>
class Item: public Element<Item<T>>
{
	using Base = Element<Item<T>>;
	friend class Element<Item<T>>;
public:
	inline static constexpr const char* Identifier = "Item";
	Key<T> key;
	Item(std::string s):Base(s.c_str()), key(s){};
	constexpr Item(const char* c): Base(c){ };
	Item* DoCreate(){return this;};
private:
	static constexpr const char* check(const char* s) { return s; }
};

class Entry: public Element<Entry>
{
	using Base = Element<Entry>;
	friend class Element<Entry>;
public:
	inline static constexpr const char* Identifier = "Entry";
    Entry(std::string s): Entry(s.c_str()){};
	constexpr Entry(const char* c): Element(c){ };
    constexpr Entry(): Element(""){};
    Entry* DoCreate(){return this;};
private:
	static constexpr const char* check(const char* s) { return s; }
};

class Name: public Element<Name>
{
	using Base = Element<Name>;
	friend class Element<Name>;
public:
    inline static constexpr const char* Identifier = "Name";
    Name(std::string s): Name(s.c_str()){};
	constexpr Name(const char* c): Element(c){ };
    constexpr Name(): Element(""){};
    Name* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
private:
	static constexpr const char* check(const char* s) { return s; }
};

template<typename D, typename U, typename TVal = double>
class Value: public Element<Value<D,U,TVal>>
{
	using Base = Element<Value<D,U,TVal>>;
	friend class Element<Value<D,U,TVal>>;
public:
	using Derived = D;
	using Unit = U;
	using TQuantity = Quantity<Unit>;
	Value(std::string s = "0.0"): Base(s.c_str()), quantity(this->to(s)) {};
	Value(TVal t): Base(std::to_string(t).c_str()), quantity(t) {};
	Value(Quantity<U> u): Base(std::to_string(u.Value()).c_str()), quantity(u) {};
	const Quantity<U>& GetQuantity() const { return this->quantity; }
	const TVal& Get() { return this->val; }
	static const char* Key;
	Base* DoCreate() { return this; };
	
	Value& operator=(const Value& a)
	{
		this->quantity = a.quantity; 
		this->val = a.val; 
		return *this; 
	}
	
	decltype(auto) ID() { return Key; }
private:
	static constexpr const char* check(const char* s) { return s; }
	Quantity<U> quantity;
	TVal val;
	String_::ParserTo<TVal> to;
};

template<typename D, typename U, typename T = double>
std::ostream& operator<<(std::ostream& out, const Value<D,U,T>& c) { return out<<D::Key<<"\t"<<c.GetQuantity(); }

class Date;

//~ #endif
