#include <sstream>
#include "../../Logger/Logger.hpp"
#include "../../Common/ParseResult.hpp"
#include "../../Common/Make/Make.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once

namespace String_
{

using Comma = T::char_<','>;
using Point = T::char_<'.'>;

template<typename Target=std::string>
decltype(auto) TryTo(std::string arg)
{
	std::stringstream buf;
	
	if(!(buf << arg) )
		return ParseResult<Target>();
	auto result = Make<Target>(buf);
	if(!result)
		return ParseResult<Target>();
	return result;
}

	
template<typename Target=std::string>
Target ParseTo(std::string arg)
{
	if constexpr (std::is_same_v<Target,double>)
	{
		Logger::Log("String_::ParseTo: comma by point in ", arg);
		std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	}
	
	auto result = TryTo<Target>(arg);

	if(!result.Valid)
		throw std::runtime_error("to<>() failed");
	return result;
}

}
