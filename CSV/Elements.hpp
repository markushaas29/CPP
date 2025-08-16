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

template<typename T = std::string>
class Number: public Element<Number<T>>
{
    using Base = Element<Number<T>>;
    friend class Element<Number<T>>;
public:
    using ValueType = T;
    using Type = Number<T>;
    inline static constexpr const char* Identifier = "Number";
    Number(const std::string& k = Identifier): Base(k){};
    template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>
    Number(TT t): Base(std::to_string(t)){};
private:
    inline static std::string check(const std::string& s) { return s; }
};

class StreetNumber: public Element<StreetNumber>		
{    
	using Base = Element<StreetNumber>;
	friend class Element<StreetNumber>;
public:
    StreetNumber(const std::string& k = Identifier): Element<StreetNumber>(k){};
	inline static constexpr const char* Identifier = "Streetnumber"; 
private:
    inline static std::string check(const std::string& s) { return s; }
};

class Postcode: public Element<Postcode>		
{    
	using Base = Element<Postcode>;
	friend class Element<Postcode>;
public:
    Postcode(const std::string& k = Identifier): Element<Postcode>(k){};
	inline static constexpr const char* Identifier = "Postcode"; 
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
    inline static constexpr const char* Pattern = "Kto/IBAN";
	explicit IBAN(const std::string& c = Default): Base(c)
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
    inline static constexpr const char* Pattern = "BLZ/BIC";
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
	Entry(const std::string& c = Identifier): Base(c){  };
	virtual bool operator==(const Entry& e) const 
	{
		auto vals = split();
		for(auto v : vals)
			if(e.Data() == v)
				return true;
		return false;
	};
	template<typename T>
	decltype(auto) GetElements(){	return T::Elements(Data());	}
	inline static std::string check(const std::string& s) { return s; }
private:
	std::vector<std::string> split() const
	{
		auto s = std::string(Data());
		std::regex rgx("(\\s+)");
		std::sregex_token_iterator iter(s.begin(),s.end(),rgx,-1), end;
		std::vector<std::string> result;
		std::for_each(iter,end, [&result](const auto& s) { result.push_back(s);});
		return result;
	}
};

template<typename T, template<typename> class D>
class IndexBase: public Element<D<T>>
{
	using Base = Element<D<T>>;
	friend class Element<D<T>>; 
	friend class Matcher; 
protected:
	explicit IndexBase(const std::string& s): Base(s), id{0}{};
public:
	size_t Id() const { return id; };
private:
	friend std::ostream& operator<<(std::ostream& out, const IndexBase& e) { return out<<e.Data()<<": "<<e.id;}
	inline static std::string check(const std::string& s) { return s; }
	size_t id;
};

template<typename T>
class Index: public IndexBase<T,Index>
{
	using Base = IndexBase<T,Index>;
public:
	using IndexType = T;
	inline static std::string Identifier = T::Identifier + std::string("Index");
	explicit Index(const std::string& s = Base::Identifier): Base(s){ };
	static std::unique_ptr<IElement> CreateIndexType(const std::string& s) { return std::make_unique<IndexType>(s); }
};

template<>
class Index<int>: public IndexBase<int,Index>
{
	using Base = IndexBase<int,Index>;
public:
	inline static std::string Identifier = std::string("Int") + "Index";
	explicit Index(const std::string& s = Base::Identifier): Base(s){};
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

class Prename: public Element<Prename>		
{    
	using Base = Element<Prename>;
	friend class Element<Prename>;
public:
    Prename(const std::string& k = Identifier): Base(k){};
	inline static constexpr const char* Identifier = "Prename"; 
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Town: public Element<Town>		
{    
	using Base = Element<Town>;
	friend class Element<Town>;
public:
    Town(const std::string& k = Identifier): Base(k){};
	inline static constexpr const char* Identifier = "Town"; 
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Street: public Element<Street>		
{    
	using Base = Element<Street>;
	friend class Element<Street>;
public:
    Street(const std::string& k = Identifier): Base(k){};
	inline static constexpr const char* Identifier = "Street"; 
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
	Empty(const std::string& c = ""): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Header: public Element<Header>
{
	using Base = Element<Header>;
	friend class Element<Header>;
public:
    inline static constexpr const char* Identifier = "Header";
	Header(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Text: public Element<Text>
{
	using Base = Element<Text>;
	friend class Element<Text>;
public:
    inline static constexpr const char* Identifier = "Text";
	Text(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

class Client: public Element<Client>
{
	using Base = Element<Client>;
	friend class Element<Client>;
public:
    inline static constexpr const char* Identifier = "Client";
    inline static constexpr const char* Pattern = "Auftraggeber";
	Client(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};


class Receiver: public Element<Receiver>
{
	using Base = Element<Receiver>;
	friend class Element<Receiver>;
public:
    inline static constexpr const char* Identifier = "Receiver";
    inline static constexpr const char* Pattern = "Empfänger";
	Receiver(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};

class BookingText: public Element<BookingText>
{
	using Base = Element<BookingText>;
	friend class Element<BookingText>;
public:
    inline static constexpr const char* Identifier = "BookingText";
    inline static constexpr const char* Pattern = "Buchungstext";
	BookingText(const std::string& c = Identifier): Base(c){ };
private:
	inline static std::string check(const std::string& s) { return s; }
};
