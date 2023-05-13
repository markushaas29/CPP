#include <sstream>
#include "../Logger/Logger.hpp"

#pragma once

template<typename Target=std::string>
struct ParseResult
{
	ParseResult(Target t): Valid{true}, Value{t} {};
	ParseResult(bool v = false): Valid{v}, Value{} {};
	Target Value;
	bool Valid;
	operator Target () { return Value; }
	operator bool () { return Valid; }
	
	std::ostream& Display(std::ostream& out) const	{	return out<<"Valid: "<<Valid<<"\t Value:"<<Value; }
};

template<typename T>
std::ostream& operator<<(std::ostream& strm, const ParseResult<T>& p){	return p.Display(strm);}
