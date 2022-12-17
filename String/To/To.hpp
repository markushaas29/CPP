#include <sstream>

#pragma once

namespace String_
{

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

template<typename Target=std::string, typename Source=std::string>
decltype(auto) TryTo(Source arg)
{
	std::stringstream buf;
	Target result;
	
	if(!(buf << arg) || !(buf >> result) || !(buf >> std::ws).eof())
		return ParseResult<Target>();
	return ParseResult<Target>(result);
}

	
template<typename Target=std::string, typename Source=std::string>
Target ParseTo(Source arg)
{
	std::stringstream buf;
	Target result;
	
	if(!(buf << arg) || !(buf >> result) || !(buf >> std::ws).eof())
		throw std::runtime_error("to<>() failed");
	return result;
}

}
