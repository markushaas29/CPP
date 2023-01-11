#include "../String/String_.hpp"
#include "../String/StringParser.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Common/DateTimes.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "Parser.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTERCONFIGURATION_HPP
#define COUNTERCONFIGURATION_HPP

template<typename Derived, typename N,typename U>
struct CounterType
{
	using Unit = U;
	using Type = Derived;
	inline static constexpr const char* Name = N::Id;
};

struct EnergyName	{	inline static constexpr const char* Id = "Energy"; };
struct Energy: public CounterType<Energy,EnergyName,Current>{};

struct WaterName	{	inline static constexpr const char* Id = "Water"; };
struct Water: public CounterType<Water,WaterName,Volume>{ };

struct GasName	{	inline static constexpr const char* Id = "Gas"; };
struct Gas: public CounterType<Gas,GasName,Volume>{	};

struct None {	inline static constexpr const char* Value = ""; };
struct Hot	{	inline static constexpr const char* Value = "_Hot";	};
struct Cold	{	inline static constexpr const char* Value = "_Cold"; };

template<typename T>
struct AdditionalInformation	{	inline static constexpr const char* Value = T::Value;	};

template<typename MeterType,  int No = 0, typename U = typename MeterType::Unit, typename A = AdditionalInformation<None>>
struct CounterConfiguration
{
	inline static constexpr uint Size = 5;
	inline static constexpr uint Number = No;
	inline static constexpr const char* AdditionalInformation = A::Value;
	inline static std::string CounterName = String_::FromInt(No) + "_" + std::string(MeterType::Name) + std::string(AdditionalInformation);
	using MeterT = MeterType;
	using Unit = U;
		
	static std::ostream& Display(std::ostream& out) { 	out<<"Number\t"<<Number<<std::endl;
														out<<"Name\t"<<CounterName<<std::endl;
														out<<"Type\t"<<MeterType::Name<<std::endl;
														out<<"Unit\t"<<U::Sign()<<std::endl;
														out<<"SiUnit\t"<<U::SiUnit()<<std::endl;
														return out;	}
};

template<typename MeterType,  int No = 0, typename U = typename MeterType::Unit, typename A = AdditionalInformation<None>>
std::ostream& operator<<(std::ostream& strm, const CounterConfiguration<MeterType,No,U,A>& c){	return c.Display(strm);}

using GasConfiguration = CounterConfiguration<Gas,1202757, Volume>;
using EnBWHouseEnergyConfiguration = CounterConfiguration<Energy,21740069, Work>;
using EnBWHallEnergyConfiguration = CounterConfiguration<Energy,11256983, Work>;
using VattenfallEnergyConfiguration = CounterConfiguration<Energy,11144078, Work>;
using AllWaterConfiguration = CounterConfiguration<Water,18061860, Volume>;
using OWaterConfiguration = CounterConfiguration<Water,279638, Volume>;
using OutWaterConfiguration = CounterConfiguration<Water,458711, Volume>;
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume, AdditionalInformation<Hot>>;
using Bottom_CWaterConfiguration = CounterConfiguration<Water,15007241, Volume, AdditionalInformation<Cold>>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,23267492, Volume, AdditionalInformation<Hot>>;
using Middle_CWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Cold>>;
using Top_CWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Cold>>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume, AdditionalInformation<Hot>>;

#endif
