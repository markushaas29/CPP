#include<cassert> 
#include<iostream> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#pragma once

template<std::size_t N>
class Literal 
{
public:
	const char* Value() { return value; }
private:
	char value[N];
	friend std::ostream& operator<<(std::ostream& s, const Literal& l) { return s<<l.value; }
};

template<std::size_t N> Literal(const char(&)[N]) -> Literal<N>;
