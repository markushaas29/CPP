#include <sstream>
#include "../../Logger/Logger.hpp"
#include "../../Common/ParseResult.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once

namespace String_
{

using Comma = T::char_<','>;
using Point = T::char_<'.'>;

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
	if constexpr (std::is_same_v<Target,double> && std::is_same_v<Source,std::string>)
	{
		Logger::Log("String_::ParseTo: comma by point in ", arg);
		std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	}
	
	auto result = TryTo<Target,Source>(arg);

	if(!result.Valid)
		throw std::runtime_error("to<>() failed");
	return result;
}

}
