#include<cassert> 
#include<cassert> 
#include<array> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#pragma once

template< auto N>
struct Literal 
{
	constexpr Literal(const char(&s)[N]) 
	{
		for(int i= 0; i<N; ++i)
			value[i] = s[i];
	}
	static constexpr std::size_t Size = N;
	decltype(auto) Value() const { return value; }
	std::array<char,N> value;
	friend std::ostream& operator<<(std::ostream& s, const Literal& l) 
	{ 
		std::copy(l.value.cbegin(), l.value.cend(), std::ostream_iterator<char>(s, ""));
    	return s;
	}
};

template<std::size_t N> Literal(const char(&)[N]) -> Literal<N>;
