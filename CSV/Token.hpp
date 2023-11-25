#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#pragma once
class IBAN;
class BIC;
class Entry;
template<typename> class Value;
class Date;
//class ;
//class ;
class IToken
{
public:
	virtual const std::string_view Data() const  = 0;	
	virtual const std::regex Pattern() const  = 0;	
	virtual bool Match(const std::string&) const  = 0;	
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string&) const  = 0;	
	virtual std::unique_ptr<IElement> Create(const std::string&) const  = 0;	
	constexpr bool operator==(const IToken& e) const{ return Data() == e.Data(); };
private:
};
//--------------------------------Token------------------------------------------------

template<typename D, typename T = int>
class Token: public IToken
{
	using Derived = D;
	using Type = T;
public:
	inline static const std::string Identifier = "Token";
 	Token(const std::string& s = ""): exclude{s} { };

	const std::string_view Data() const  {	return Derived::Pattern; };	
	bool Match(const std::string& s) const  {	return exclude == "" ? std::regex_match(s,pattern) : std::regex_match(s,pattern) && !std::regex_match(s,std::regex(exclude)); };	
	virtual std::unique_ptr<IElement> Create(const std::string& s) const  { return std::make_unique<Type>(s); };	
	virtual const std::regex Pattern() const { return pattern; };	
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string& s) const  
	{
		std::vector<std::unique_ptr<IElement>> result;
		std::smatch match;

		std::regex rgx(";");
		std::sregex_token_iterator iter(s.begin(),s.end(), rgx, -1);
		std::sregex_token_iterator end;
		for ( ; iter != end; ++iter)
		{
			std::string si(*iter);
			if (Match(si))
				result.push_back(std::make_unique<Type>(si));
		}
		return result;
	};	

	bool operator==(const Token& e) const{ return Data() == e.Data(); };
protected:
	using Base = Token<D,T>;
private:
	std::regex pattern = std::regex( Derived::Pattern );
	std::string exclude;
};

std::ostream& operator<<(std::ostream& out, const IToken& e) {	return out<<e.Data();}

class DateToken: public Token<DateToken, Date>
{
public:
	inline static constexpr const char* Pattern = "(0?[1-9]|[1-2][0-9]|3[0-1]).(0?[1-9]|1[0-2]).(\\d{4})";
};

class EntryToken: public Token<EntryToken, Entry>
{
public:
	inline static constexpr const char* Pattern = "";
};

class IBANToken: public Token<IBANToken, IBAN>
{
public:
	IBANToken(const std::string& e = ""): Base(e) {}
	inline static constexpr const char* Pattern = "^DE\\d{20}$";
};

class BICToken: public Token<BICToken, BIC>
{
public:
	inline static constexpr const char* Pattern = "([a-zA-Z]{4}[a-zA-Z]{2}[a-zA-Z0-9]{2}([a-zA-Z0-9]{3})?)";
};

class SumToken: public Token<SumToken, Quantity<Sum>>
{
public:
	inline static constexpr const char* Pattern = "((^[-+]?((\\d+[.,]\\d{2})|([1-9]\\d+))€$)|(^[-+]?((\\d+[.,]\\d{2}))$))";
};

class ValueToken: public Token<ValueToken, Value<int>>
{
public:
	inline static constexpr const char* Pattern = "";
};

class WordToken: public Token<WordToken, Entry>
{
public:
	inline static constexpr const char* Pattern = "[a-zA-z]+";
};

class IDToken: public Token<IDToken, ID<int>>
{
public:
	inline static constexpr const char* Pattern = "^[0-9]+$";
};

class DateIndexToken: public Token<DateIndexToken, Date>
{
public:
	inline static constexpr const char* Pattern = "Buchungstag";
};

class QuantityToken: public Token<QuantityToken, Entry>
{
public:
	inline static constexpr const char* Pattern = "";
};

class KeyValueToken: public Token<KeyValueToken, Entry>
{
public:
	inline static constexpr const char* Pattern = "([a-zA-z])+\\s?:\\s?([\\w\\d]+)";
};
