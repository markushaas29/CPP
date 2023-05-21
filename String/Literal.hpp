#include<cassert> 
#include<iostream> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#pragma once

template<std::size_t N>
struct Literal 
{
	constexpr Literal(const char(&s)[N]) { std::copy_n(s,N,value); } 
	const char* Value() { return value; }
	char value[N];
	friend std::ostream& operator<<(std::ostream& s, const Literal& l) { return s<<l.value; }
};

template<std::size_t N> Literal(const char(&)[N]) -> Literal<N>;
