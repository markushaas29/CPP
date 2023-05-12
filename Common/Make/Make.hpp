#include <sstream>
#include "../DateTimes.hpp"
#include "../../Logger/Logger.hpp"
#include "../../String/To/To.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once

template<typename Target=std::string>
decltype(auto) TryMake(std::istream& arg)
{
	Target result;
	if(!(arg >> result) || !(arg >> std::ws).eof())
		return String_::ParseResult<Target>();
	return String_::ParseResult<Target>(result);
}

template<>
decltype(auto) TryMake<DateTimes::Date>(std::istream& arg)
{
	using Target = DateTimes::Date;
	auto result = DateTimes::Date::Create(arg);
	if(!result.Valid())
		return String_::ParseResult<Target>();
	return String_::ParseResult<Target>(result);
}

template<typename Target>
Target Make(std::string arg)
{
//	if constexpr (std::is_same_v<Target,double> && std::is_same_v<Source,std::string>)
//	{
//		Logger::Log("String_::ParseTo: comma by point in ", arg);
//		std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
//	}
//	
//	auto result = TryTo<Target,Source>(arg);
//
//	if(!result.Valid)
//		throw std::runtime_error("to<>() failed");
//	return result;
	std::cout<<"String"<<std::endl;
	return Target{};
}

template<typename Target>
decltype(auto) Make(std::istream& is)
{
	auto result = TryMake<Target>(is);
	if(!result.Valid)
		throw std::runtime_error("Make() failed");
	return result;
}

decltype(auto) Make(std::istream& is)
{
	auto result = TryMake<DateTimes::Date>(is);
	if(!result.Valid)
		throw std::runtime_error("Make() failed");
	return result;
}
