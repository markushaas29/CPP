#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
//~ #include <source_location>

#ifndef EXPECT_HPP
#define EXPECT_HPP

class Expect
{  
};

template<typename T>
constexpr void print(T t) { std::cout<<t<<std::endl; }

enum class ErrorAction {Ignore, Throwing, Termiating, Logging};

constexpr ErrorAction defaultErrorActon = ErrorAction::Logging;

template<class LogLevel = Debug, ErrorAction action = defaultErrorActon,class C>
//~ constexpr void expect(C cond, const std::source_location location =  std::source_location::current())
constexpr bool expect(C cond, const std::string& message)
{
	if constexpr(action == ErrorAction::Logging)
	{
		if(!cond)
		{
			Logger::Log<LogLevel>(message);
			return false;
		}
		return true;
	}
	if constexpr(action == ErrorAction::Throwing)
	{	
		if(!cond)
			throw "Test";
		return true;
	}	
}

#endif
