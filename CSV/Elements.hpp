#include <string>
#include <map>
#include <chrono>
#include <regex>
#include "Element.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Validator/Validator.hpp"

#pragma once

template<typename T = std::string>
class ID: public Element<ID<T>>
{
	using Base = Element<ID<T>>;
	friend class Element<ID<T>>;
public:
	using ValueType = T;
	using Type = ID<T>;
	inline static constexpr const char* Identifier = "ID";
	ID(const std::string& k = Identifier): Base(k){};
	template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>	
	ID(TT t): Base(std::to_string(t)){};
private:
	inline static std::string check(const std::string& s) { return s; }
};

class IBAN: public Element<IBAN>
{
	using Base = Element<IBAN>;
	friend class Element<IBAN>;
public:
	inline static constexpr uint Length = 22;
	inline static constexpr const char* Default = "XX00000000000000000000";
	inline static constexpr const char* Identifier = "IBAN";
	IBAN(const std::string& c = Default): Base(c)
	{
		if(!isValid(c))
			Logger::Log<Error>("IBAN",c," is invalid!");
	};
	IBAN* DoCreate(){return this;};
	bool Valid() { return std::string(Default) != Data(); }
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


class BIC: public Element<BIC>
{
	using Base = Element<BIC>;
	friend class Element<BIC>;
public:
	inline static constexpr const char* Identifier = "BIC";
	BIC(const std::string& c=Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Entry: public Element<Entry>
{
	using Base = Element<Entry>;
	friend class Element<Entry>;
public:
	inline static constexpr const char* Identifier = "Entry";
	Entry(const std::string& c): Base(c){ };
	void split()
	{
		auto s = std::string(Data());
		std::regex rgx("(\\s+)");
		std::sregex_token_iterator iter(s.begin(),s.end(),rgx,-1);
		std::sregex_token_iterator end;
		for ( ; iter != end; ++iter)
			std::cout << *iter << '\n';
	}
	inline static std::string check(const std::string& s) { return s; }
private:
};

class Index: public Element<Index>
{
	using Base = Element<Index>;
	friend class Element<Index>; 
	friend class Matcher; 
public:
	inline static constexpr const char* Identifier = "Index";
	explicit Index(const std::string& s = Identifier): Base(s), id{0}{};
	size_t Id() const { return id; };
private:
	friend std::ostream& operator<<(std::ostream& out, const Index& e) { return out<<e.Data()<<": "<<e.id;}
	inline static std::string check(const std::string& s) { return s; }
	size_t id;
};

class Name: public Element<Name>
{
	using Base = Element<Name>;
	friend class Element<Name>;
public:
    inline static constexpr const char* Identifier = "Name";
	Name(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

template<typename T>//, typename U, typename TVal = double>
class Value: public Element<Value<T>>
{
	using Base = Element<Value<T>>;
	friend class Element<Value<T>>;
public:
	inline static constexpr const char* Identifier = "Value";
	Value(T t): Base(std::to_string(t)) {};
	Value(const std::string& s = Identifier): Base(s) {};
private:
	inline static std::string check(const std::string& s) { return s; }
	T val;
};

class Empty: public Element<Empty>
{
	using Base = Element<Empty>;
	friend class Element<Empty>;
public:
    inline static constexpr const char* Identifier = "Empty";
	Empty(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return Identifier; }
};
