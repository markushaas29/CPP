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

template<typename T = std::string>
class Key: public Element
{
	friend class Element;
public:
	using ValueType = T;
	using Type = Key<T>;
	inline static constexpr const char* Identifier = "Key";
	Key(std::string s = ""): Element(s.c_str()) {};
	explicit operator std::string(){return this->Value();}
	Key(const Key& k): Element(k.Value().c_str()){};
	Key(const char* k): Element(k){};

	bool operator==(T s) const{ return s == this->Value(); }
	bool operator<=>(T s) const{ return s <=> this->Value(); }
	constexpr const char* check(const char* s) { return s; }
private:
};

template<typename T = std::string>
inline decltype(auto) operator<=> (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() <=> rhs.Value(); }

template<typename T = std::string>
inline bool operator== (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Value() == rhs.Value(); }

class IBAN: public Element
{
	friend class Element;
public:
	inline static constexpr uint Length = 22;
	inline static constexpr const char* Default = "XX00000000000000000000";
	inline static constexpr const char* Identifier = "IBAN";
	IBAN(std::string s): IBAN{s.c_str()}
	{
		if(!isValid(s.c_str()))
			Logger::Log<Error>("IBAN",s," is invalid!");
	};
	IBAN(const char* c): Element(check(c)){	};
	IBAN(): Element(""){ };
	IBAN* DoCreate(){return this;};
	bool Valid() { return std::string(Default) != Value(); }
	decltype(auto) ID() { return Identifier; }
	static constexpr bool isValid(const char* iban)
	{
		if (!SizeValidator<Length>::Condition(iban) || !LetterValidator::Condition(iban[0]) || !LetterValidator::Condition(iban[1]))
			return false;

		for(int i = 2; i < Length; ++i)
			if(!NumberValidator::Condition(iban[i]))
				return false;
			
		return true;
	}
	constexpr const char* check(const char* iban) { return isValid(iban) ? iban : Default; }
private:
};

inline bool operator< (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() < rhs.Value(); }
inline bool operator== (const IBAN& lhs, const IBAN& rhs){ return lhs.Value() == rhs.Value(); }

class BIC: public Element
{
	friend class Element;
public:
	inline static constexpr const char* Identifier = "BIC";
	BIC(std::string s): BIC(s.c_str()){};
	BIC(const char* c): Element(check(c)){ };
	BIC(): Element(""){ };
	BIC* DoCreate(){return this;};
	constexpr const char* check(const char* s) { return s; }
private:
};


template<typename T = std::string>
class Item: public Element
{
	friend class Element;
public:
	inline static constexpr const char* Identifier = "Item";
	Key<T> key;
	Item(std::string s):Element(s.c_str()), key(s){};
	Item(const char* c): Element(check(c)){ };
	Item* DoCreate(){return this;};
private:
	constexpr const char* check(const char* s) { return s; }
};

class Entry: public Element
{
	friend class Element;
public:
	inline static constexpr const char* Identifier = "Entry";
    Entry(std::string s): Entry(s.c_str()){};
	 Entry(const char* c): Element(check(c)){ };
     Entry(): Element(""){};
    Entry* DoCreate(){return this;};
	constexpr const char* check(const char* s) { return s; }
private:
};

class Name: public Element
{
	friend class Element;
public:
    inline static constexpr const char* Identifier = "Name";
    Name(std::string s): Name(s.c_str()){};
	Name(const char* c): Element(check(c)){ };
    Name(): Element(""){};
    Name* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
	constexpr const char* check(const char* s) { return s; }
private:
};

template<typename D, typename U, typename TVal = double>
class Value: public Element
{
	friend class Element;
public:
	using Derived = D;
	using Unit = U;
	using TQuantity = Quantity<Unit>;
	Value(std::string s = "0.0"): Element(s.c_str()), quantity(this->to(s)) {};
	Value(TVal t): Element(std::to_string(t).c_str()), quantity(t) {};
	Value(Quantity<U> u): Element(std::to_string(u.Value()).c_str()), quantity(u) {};
	const Quantity<U>& GetQuantity() const { return this->quantity; }
	const TVal& Get() { return this->val; }
	static const char* Key;
	Element* DoCreate() { return this; };
	
	Value& operator=(const Value& a)
	{
		this->quantity = a.quantity; 
		this->val = a.val; 
		return *this; 
	}
	
	decltype(auto) ID() { return Key; }
	constexpr const char* check(const char* s) { return s; }
private:
	Quantity<U> quantity;
	TVal val;
	String_::ParserTo<TVal> to;
};

template<typename D, typename U, typename T = double>
std::ostream& operator<<(std::ostream& out, const Value<D,U,T>& c) { return out<<D::Key<<"\t"<<c.GetQuantity(); }

class Date;

//~ #endif
