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

template<typename D>
class Token: public IToken
{
	using Derived = D;
public:
	inline static const std::string Identifier = "Token";
 	Token() { };

	const std::string_view Data() const  {	return Derived::Pattern; };	
	bool Match(const std::string& s) const  
	{ 
		std::smatch m;
		return std::regex_match(s,m,pattern); 
	};	
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
