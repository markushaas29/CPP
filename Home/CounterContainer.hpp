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
#include "Counter.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef COUNTERCONTAINER_HPP
#define COUNTERCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename List>
class CounterContainer{};

template<typename Head>
class CounterContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head>;
	using ContainerType = CounterContainer<Typelist<Head>>;

	static std::ostream& Display(std::ostream& os) 	{	return Type::Display(os);	}
	void Write(const std::string sourcePath = ".")	{	Type::Write(sourcePath);	}
	void Read(const std::string sourcePath = ".")	{	Type::Read();	}
	
	template<unsigned N>
	auto Get() { return At<CounterTypes,N>::Type; }

	static CounterContainer& Instance()
	{
		static CounterContainer instance;
		return instance;
	}	
protected:
	CounterContainer() { Logger::Log<Info>()<<"CounterContainer created."<<std::endl; };
	~CounterContainer()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
	CounterContainer& operator=(const CounterContainer&) = delete;
	CounterContainer(const CounterContainer& c) = delete;
};

template<typename Head, typename... Tail>
class CounterContainer<Typelist<Head,Tail...>>: public CounterContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using CounterTypes = Typelist<Head,Tail...>;
	using ContainerType = CounterContainer<Typelist<Head,Tail...>>;
	using Base = CounterContainer<Typelist<Tail...>>;
	
	static std::ostream& Display(std::ostream& os) 	{	return Base::Display(Type::Display(os));	}
	void Write(const std::string sourcePath = ".")	
	{	
	}
	
	void Read(const std::string sourcePath = ".")
	{
	}
	
	template<unsigned N>
	auto Get() 
	{ 
		using C = At<CounterTypes,N>; 
		return C::Number;
	}

	static CounterContainer& Instance()
	{
		static CounterContainer instance;
		return instance;
	}	
protected:
	CounterContainer() { Logger::Log<Info>()<<"CounterContainer created."<<std::endl; };
	~CounterContainer()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
	CounterContainer& operator=(const CounterContainer&) = delete;
	CounterContainer(const CounterContainer& c) = delete;
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const CounterContainer<Head,Tail...>& c){	return c.Display(strm); }


using CG1 = Counter<GasConfiguration>;
using CEHouse = Counter<EnBWHouseEnergyConfiguration>;
using CEHall = Counter<EnBWHallEnergyConfiguration>;
using CVat = Counter<VattenfallEnergyConfiguration>;
using CWA = Counter<AllWaterConfiguration>;
using CWO = Counter<OWaterConfiguration>;
using CWOut = Counter<OutWaterConfiguration>;

using CBHW = Counter<Bottom_HWaterConfiguration>;
using CBCW = Counter<Bottom_CWaterConfiguration>;
using CMCW = Counter<Middle_CWaterConfiguration>;
using CMHW = Counter<Middle_HWaterConfiguration>;
using CTCW = Counter<Top_CWaterConfiguration>;
using CTHW = Counter<Top_HWaterConfiguration>;

using CounterContainerType = CounterContainer<Typelist<CEHouse, CEHall,CVat,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW>>::ContainerType;

#endif
