#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Expect/Expect.hpp"
//~ #include <source_location>

#ifndef VAILDATOR_HPP
#define VALIDATOR_HPP

template<typename T, typename LogLevel = Error>
class Vaildator
{  
public:
	using ValidationType = std::function<bool(T)>;
	T operator ()(T t) 
	{
		if(validation(t))
			return t;
		
		expect<LogLevel>(false,"Invalid");
		return returnValue;
	};
protected:
	Vaildator(ValidationType v, const T r = T{}): validation{v}, returnValue{r}{};
private:
	const T returnValue;
	const ValidationType validation; 
};

template<typename T>
class RangeValidator: public Vaildator<T>
{
	using Base = Vaildator<T>;
public:
	RangeValidator(const T min, const T max): Base{[&](const T t) { return t >= min && t <= max; },min},  min{min}, max{max} {}
private:
	Base::ValidationType validation = [&](const T t) { return t >= min && t <= max; };
	const T min;
	const T max;
};

#endif
