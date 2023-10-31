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

//template<typename T = std::string>
//class Key: public Element
//{
//	friend class Base;
//public:
//	using ValueType = T;
//	using Type = Key<T>;
//	inline static constexpr const char* Identifier = "Key";
//	explicit operator std::string(){return this->Get();}
//	Key(const Key& k): Base(k.Get().c_str()){};
//	Key(const std::string k): Base(k){};
//
//	bool operator==(T s) const{ return s == this->Get(); }
//	bool operator<=>(T s) const{ return s <=> this->Get(); }
//private:
//	constexpr const std::string* check(const std::string* s) { return s; }
//};
//
//template<typename T = std::string>
//inline decltype(auto) operator<=> (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Get() <=> rhs.Get(); }

//template<typename T = std::string>
//inline bool operator== (const Key<T>& lhs, const Key<T>& rhs){ return lhs.Get() == rhs.Get(); }

class IBAN: public Element<IBAN>
{
	using Base = Element<IBAN>;
	friend class Element<IBAN>;
public:
	inline static constexpr uint Length = 22;
	inline static constexpr const char* Default = "XX00000000000000000000";
	inline static constexpr const char* Identifier = "IBAN";
	IBAN(const std::string& c): Base(c)
	{
		if(!isValid(c))
			Logger::Log<Error>("IBAN",c," is invalid!");
	};
	IBAN* DoCreate(){return this;};
	bool Valid() { return std::string(Default) != Get(); }
	decltype(auto) ID() { return Identifier; }
	static bool isValid(const std::string& iban)
	{
		if (iban.size() != 22 || !LetterValidator::Condition(iban[0]) || !LetterValidator::Condition(iban[1]))
			return false;

		for(int i = 2; i < Length; ++i)
			if(!NumberValidator::Condition(iban[i]))
				return false;
			
		return true;
	}
private:
	inline static std::string check(const std::string& iban) { return isValid(iban) ? iban : Default; }
};

inline bool operator< (const IBAN& lhs, const IBAN& rhs){ return lhs.Get() < rhs.Get(); }
inline bool operator== (const IBAN& lhs, const IBAN& rhs){ return lhs.Get() == rhs.Get(); }

class BIC: public Element<BIC>
{
	using Base = Element<BIC>;
	friend class Element<BIC>;
public:
	inline static constexpr const char* Identifier = "BIC";
	BIC(const std::string& c): Base(c){ };
	BIC* DoCreate(){return this;};
	inline static std::string check(const std::string& s) { return s; }
private:
};


//template<typename T = std::string>
//class Item: public Element
//{
//public:
//	inline static constexpr const char* Identifier = "Item";
//	Key<T> key;
//	Item(const std::string& c): Base(check(c)){ };
//	Item* DoCreate(){return this;};
//private:
//	constexpr const std::string& check(const std::string& s) { return s; }
//};
//
class Entry: public Element<Entry>
{
	using Base = Element<Entry>;
	friend class Element<Entry>;
public:
	inline static constexpr const char* Identifier = "Entry";
	Entry(const std::string& c): Base(c){ };
    Entry* DoCreate(){return this;};
	inline static std::string check(const std::string& s) { return s; }
private:
};
//
class Name: public Element<Name>
{
	using Base = Element<Name>;
	friend class Element<Name>;
public:
    inline static constexpr const char* Identifier = "Name";
	Name(const std::string& c): Base(c){ };
    Name* DoCreate(){return this;};
	decltype(auto) ID() { return Identifier; }
	inline static std::string check(const std::string& s) { return s; }
private:
};
//
template<typename T>//, typename U, typename TVal = double>
class Value: public Element<Value<T>>
{
	using Base = Element<Value<T>>;
	friend class Element<Value<T>>;
public:
	Value(T t): Base(t) {};
	Value(const std::string& s): Base(s) {};
	inline static std::string check(const std::string& s) { return s; }
private:
	T val;
};
//
//template<typename D, typename U, typename T = double>
//std::ostream& operator<<(std::ostream& out, const Value<D,U,T>& c) { return out<<D::Key<<"\t"<<c.GetQuantity(); }
//
//class Date;

//~ #endif
