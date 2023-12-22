#include <map>
#include <chrono>
#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename... T>
class Registration
{
public:
	inline static constexpr const char TypeIdentifier[] = "Registration";
	inline static constexpr Literal TypeId{TypeIdentifier};

private:
	friend std::ostream& operator<<(std::ostream& s, const Registration& c){return s;}
};
