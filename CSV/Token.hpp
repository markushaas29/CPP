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
 	Token() { };

	const std::string_view Data() const  {	return Derived::Pattern; };	
	bool Match(const std::string& s) const  {	return std::regex_match(s,pattern); };	
	virtual const std::regex Pattern() const { return pattern; };	

	bool operator==(const Token& e) const{ return Data() == e.Data(); };
private:
	std::regex pattern = std::regex( Derived::Pattern );
};

std::ostream& operator<<(std::ostream& out, const IToken& e) {	return out<<e.Data();}

class DateToken: public Token<DateToken>
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
	inline static constexpr const char* Pattern = "";
};

class BICToken: public Token<BICToken, BIC>
{
public:
	inline static constexpr const char* Pattern = "";
};

class SumToken: public Token<SumToken, Quantity<Sum>>
{
public:
	inline static constexpr const char* Pattern = "";
};

class ValueToken: public Token<ValueToken, Value<int>>
{
public:
	inline static constexpr const char* Pattern = "";
};

class WordToken: public Token<WordToken>
{
public:
	inline static constexpr const char* Pattern = "";
};

class QuantityToken: public Token<QuantityToken>
{
public:
	inline static constexpr const char* Pattern = "";
};
