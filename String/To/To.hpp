#include <sstream>
#include "../String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Common/ParseResult.hpp"
#include "../../Common/Make/Make.hpp"
#include "../../Wrapper/Wrapper.hpp"
#include "../../Is/Is.hpp"
#include "../../String/Literal.hpp"
#include "../../String/Format.hpp"

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
	static constexpr const char TypeIdentifier[] = "To<>()";
    static constexpr Literal LiteralType{TypeIdentifier};

	if(arg.size() == 0)
		return Target{0};

	std::string info{arg};
	if constexpr (std::is_same_v<Target,double>)
	{
		Logger::Log("String_::ParseTo: comma by point in ", arg);
		if(Contains(arg,".") && Contains(arg,","))
			arg.erase(std::remove(arg.begin(), arg.end(), '.'), arg.end());
		std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	}
	
	auto result = TryTo<Target>(arg);
	if(!result.Valid)
		Is<Throwing,LiteralType>(Format(" failed: argument ",info, " formatted to ", arg))(false);
	return result;
}

}
