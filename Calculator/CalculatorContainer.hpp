#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef CALCULATORCONTAINER_HPP
#define CALCULATORCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CalculatorContainer----------------------------------------------------------------------------------------

template<typename List>
class CalculatorContainer{};

template<typename Head>
class CalculatorContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using CalculatorTypes = Typelist<Head>;
	using ContainerType = CalculatorContainer<Typelist<Head>>;
protected:
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Type::Display(os);
	}
	
	void Calculate()
	{
		Head::Calculate();
	}		
};

template<typename Head, typename... Tail>
class CalculatorContainer<Typelist<Head,Tail...>>: public CalculatorContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CalculatorTypes = Typelist<Head,Tail...>;
	using ContainerType = CalculatorContainer<Typelist<Head,Tail...>>;
	using Base = CalculatorContainer<Typelist<Tail...>>;
protected:
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Base::Display(Type::Display(os));		
	}
	
	void Write(const std::string sourcePath = ".")
	{
		Type::Write(sourcePath);
		Base::Write(sourcePath);		
	}
	
	void Calculate()
	{
		Head::Calculate();
		Base::Calculate();
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const CalculatorContainer<Head,Tail...> c)
{
	return c.Display(strm);
}


#endif
