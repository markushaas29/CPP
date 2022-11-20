#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
//~ #include <source_location>

#ifndef EXPECT_HPP
#define EXPECT_HPP

class Expect
{  
};

enum class ErrorAction {Ignore, Throwing, Termiating, Logging};

constexpr ErrorAction defaultErrorActon = ErrorAction::Throwing;

template<ErrorAction action = defaultErrorActon, class Logging = Debug,class C>
//~ constexpr void expect(C cond, const std::source_location location =  std::source_location::current())
constexpr void expect(C cond)
{
	if constexpr(action == ErrorAction::Logging)
		if(!cond)
			Logger::Log<Info>("Test");
	if constexpr(action == ErrorAction::Throwing)
		if(!cond)
			throw "Test";
}

#endif
