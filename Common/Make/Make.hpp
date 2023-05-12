#include <sstream>
#include "../../Logger/Logger.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once

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
Target Make(std::istream& is)
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
	std::cout<<"istream"<<std::endl;
	return Target{};
}
