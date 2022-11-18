#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef EXPECT_HPP
#define EXPECT_HPP

class Expect
{  
};

enum class ErrorAction {Ignore, Throwing, Termiating, Logging};

constexpr ErrorAction defaultErrorActon = ErrorAction::Throwing;

template<ErrorAction action = defaultErrorActon, class C>
constexpr void expect(C cond, uint code)
{
	if constexpr(action == ErrorAction::Logging)
		if(!cond)
			Logger::Log<Info>("Test");
	if constexpr(action == ErrorAction::Throwing)
		if(!cond)
			throw "Test";
}

#endif
