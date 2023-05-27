#include<array> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#pragma once

template< auto N>
constexpr decltype(auto) ToArray(const char(&s)[N])
{
	std::array<char,N> value;
	for(int i= 0; i<N; ++i)
		value[i] = s[i];

	return value;
}

constexpr std::size_t Len(const char * a)
{
	const char* end = a;
	while(*end) ++end;

	return end - a;
}
	
template<auto N>	
constexpr std::size_t Len(std::array<char,N> a)
{
	auto end = a.cbegin();
	while(*end) ++end;

	return end - a.cbegin();
}

template<auto N>	
constexpr decltype(auto) Init(const char * a)
{
	std::array<char,N> result = {};
	int i = 0;
	const char* end = a;
	
	while(*end) 
	{
		result[i++] = *end;
		++end;
	}
	
	return result;
}
