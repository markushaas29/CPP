#include <sstream>
#include "../String_.hpp"
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
	auto tm = TryMake<Target>();
	auto result = tm(buf);
	return result;
}

	
template<typename Target=std::string>
Target ParseTo(std::string arg)
{
	if(arg.size() == 0)
		return Target{0};

	if constexpr (std::is_same_v<Target,double>)
	{
		Logger::Log("String_::ParseTo: comma by point in ", arg);
		if(Contains(arg,".") && Contains(arg,","))
		{
			std::replace( arg.end()-4, arg.end(), Comma::Value, Point::Value);
			arg.erase(std::remove(arg.begin(), arg.end()-4, '.'), arg.end()-4);
		}
		else
			std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	}
	
	auto result = TryTo<Target>(arg);
	if(!result.Valid)
		throw std::runtime_error("to<>() failed");
	return result;
}

}
