#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Expect/Expect.hpp"
#include <string.h>
//~ #include <source_location>

#pragma once
template<typename Derived, typename Type,typename Level = Error>
class Validator
{ 
public:
	static constexpr bool Condition(Type t) {	return Derived::condition(t); }	
	
	static constexpr Type Check(Type t, Type defaultValue = returnValue) 
	{
		if(Condition(t))
			return t;
			
			//~ expect<Level>(false,"Invalid");
			//~ Logger::Log<Level>("Invalid Value",t);
			
		return defaultValue;
	};
private:
	static constexpr Type returnValue = Derived::returnValue;
};

template<typename T, T Min,T Max>
class RangeValidator: public Validator<RangeValidator<T,Min,Max>,T>
{
	using Base = Validator<RangeValidator<T,Min,Max>,T>;
	friend class Validator<RangeValidator<T,Min,Max>,T>;
public:
	using Type = T;
private:
	static constexpr bool condition(const T t) { return t >= min && t <= max; };
	static constexpr T min = Min;
	static constexpr T max = Max;
	static constexpr T returnValue = min;
};

class NumberValidator: public RangeValidator<char,47,57> {};
class CapitalLetterValidator: public RangeValidator<char,65,90> {};
class LowerCaseLetterValidator: public RangeValidator<char,97,122> {};

class LetterValidator: public Validator<LetterValidator, char>
{
public:
	using Type = char;
private:
	using Base = Validator<LetterValidator,Type>;
	friend class Validator<LetterValidator,Type>;
	static constexpr bool condition(const Type t) { return LowerCaseLetterValidator::Condition(t) || CapitalLetterValidator::Condition(t); };
	static constexpr Type returnValue = {}; 

};

constexpr decltype(auto) length( const char* c){  return std::char_traits<char>::length(c); }
constexpr bool IsLetter(char c) noexcept { return c > 64 && c < 91 || c > 96 && c < 123; };
constexpr bool IsNum(char c) noexcept { return c > 47 && c < 58; };

template<uint N, typename T = const char *>
class SizeValidator: public Validator<SizeValidator<N, T>,T>
{
	using Base = Validator<SizeValidator<N, T>,T>;
	friend class Validator<SizeValidator<N, T>,T>;
public:
	using Type = T;
private:
	static constexpr bool condition(const T t) { return length(t) == N; };
	static constexpr int size = N;
	static constexpr T returnValue = nullptr; 
};

