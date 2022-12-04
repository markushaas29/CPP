#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Expect/Expect.hpp"
#include <string.h>
//~ #include <source_location>

#pragma once
template<typename Derived, typename Type,typename Level = Error>
class Vaildator
{ 
public:
	static constexpr Type Check(Type t) 
	{
		if(Derived::Condition(t))
			return t;
		
		expect<Level>(false,"Invalid");
		
		return returnValue;
	};
private:
	static constexpr Type returnValue = Derived::returnValue;
};

template<typename T, T Min,T Max>
class RangeValidator: public Vaildator<RangeValidator<T,Min,Max>,T>
{
	using Base = Vaildator<RangeValidator<T,Min,Max>,T>;
	friend class Vaildator<RangeValidator<T,Min,Max>,T>;
public:
	using Type = T;
private:
	static constexpr bool Condition(const T t) { return t >= min && t <= max; };
	static constexpr T min = Min;
	static constexpr T max = Max;
	static constexpr T returnValue = min;
};

constexpr decltype(auto) length( const char* c){  return std::char_traits<char>::length(c); }

template<uint N, typename T = const char *>
class SizeValidator: public Vaildator<SizeValidator<N, T>,T>
{
	using Base = Vaildator<SizeValidator<N, T>,T>;
	friend class Vaildator<SizeValidator<N, T>,T>;
public:
	using Type = T;
private:
	static constexpr bool Condition(const T t) { return length(t) == N; };
	static constexpr int size = N;
	static constexpr T returnValue = nullptr;
};

