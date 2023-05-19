#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
//~ #include <source_location>

#pragma once

struct CompileTime {};
struct Ignore {};
struct Throwing {};
struct Termiating {};
struct Logging {};

template<typename... T>
class Is {};

template<>
class Is<CompileTime> 
{
public:
	constexpr bool operator()(bool c)
	{
		return c;
	}
};

template<>
class Is<Logging> 
{
public:
	bool operator()(bool c)
	{
		Logger::Log("Is false");
		return c;
	}
};

template<>
class Is<Logging, Throwing> 
{
public:
	bool operator()(bool c)
	{
		Logger::Log("Is false");
		if(!c)
			throw "Test";
		return c;
	}
};
//constexpr bool expect(C cond, const std::string& message)
//{
//	if constexpr(action == ErrorAction::Logging)
//	{
//		if(!cond)
//		{
//			Logger::Log<LogLevel>(message);
//			return false;
//		}
//		return true;
//	}
//	if constexpr(action == ErrorAction::Throwing)
//	{	
//		if(!cond)
//			throw "Test";
//		return true;
//	}	
//}

