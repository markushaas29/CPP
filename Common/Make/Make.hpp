#include <sstream>
#include "../ParseResult.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once

template<typename Target=std::string>
decltype(auto) TryMake(std::istream& arg)
{
	Target result;
	if(!(arg >> result) || !(arg >> std::ws).eof())
		return ParseResult<Target>();
	return ParseResult<Target>(result);
}

template<typename Target>
decltype(auto) Make(std::istream& is)
{
	auto result = TryMake<Target>(is);
	if(!result.Valid)
		throw std::runtime_error("Make() failed");
	return result;
}
//
//decltype(auto) TryMake<DateTimes::Date>(std::istream& arg)
//{
//	using Target = DateTimes::Date;
//	auto result = DateTimes::Date::Create(arg);
//	if(!result.Valid())
//		return ParseResult<Target>();
//	return ParseResult<Target>(result);
//}
//
//template<>
//decltype(auto) Make<DateTimes::Date>(std::istream& is)
//{
//	auto result = TryMake<DateTimes::Date>(is);
//	if(!result.Valid)
//		throw std::runtime_error("Make() failed");
//	return result;
//}
