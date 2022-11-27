#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Expect/Expect.hpp"
//~ #include <source_location>

#ifndef VAILDAT_HPP
#define VALIDAT_HPP

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
	static constexpr Type returnValue = Derived::min;
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
};

#endif
