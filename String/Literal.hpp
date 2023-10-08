#include<cassert> 
#include<array> 
#include "../Common/ArrayHelper.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#pragma once

template< auto N>
struct Literal 
{
	constexpr Literal(const char(&s)[N]): Value{ToArray(s)}	{	}
	static constexpr std::size_t Size = N;
	const std::array<char,N> Value;
	constexpr decltype(auto) Ptr() const { return Value.data(); };
	constexpr bool operator==(const std::string& s) const{ return s == std::string(Value.data()); };
	friend std::ostream& operator<<(std::ostream& s, const Literal& l) 
	{ 
		std::copy(l.Value.cbegin(), l.Value.cend(), std::ostream_iterator<char>(s, ""));
    	return s;
	}
};

template<std::size_t N> Literal(const char(&)[N]) -> Literal<N>;
