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
private:
//	virtual constexpr bool operator==(const IToken& e) const = 0;
//	virtual constexpr std::strong_ordering operator<=>( const IToken& e) const noexcept = 0;
//	virtual constexpr const_iterator Begin() const = 0;
//	constexpr decltype(auto) End() { return data.begin() + size; }
//	constexpr decltype(auto) Size() { return size; }
//
//	constexpr bool operator==(const Token& e) const{ return Data() == e.Data(); };
//	constexpr std::strong_ordering operator<=>( const Token& e) const noexcept { return Data() <=> e.Data(); }
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
	virtual const std::regex Pattern() const { return pattern; };	

	bool operator==(const Token& e) const{ return Data() == e.Data(); };
	std::strong_ordering operator<=>( const Token& e) const noexcept { return Data() <=> e.Data(); }
private:
	std::regex pattern = std::regex( Derived::Pattern );
};

std::ostream& operator<<(std::ostream& out, const IToken& e) {	return out<<e.Data();}

class DateToken: public Token<DateToken>
{
public:
	inline static constexpr const char* Pattern = "(0?[1-9]|[1-2][0-9]|3[0-1]).(0?[1-9]|1[0-2]).(\\d{4})";
};
