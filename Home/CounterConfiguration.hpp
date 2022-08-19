#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
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
	static const uint Number = No;
	inline static constexpr const char* AdditionalInformation = A::Value;
	inline static std::string CounterName = String_::FromInt(No) + "_" + std::string(MeterType::Name) + std::string(AdditionalInformation);
	inline static constexpr const char* DestinationPath = "//home//markus//Downloads//";
	using MeterT = MeterType;
	using Unit = U;
};

template<typename U, typename Pre = SIPrefix<0>, typename Q = Quantity<U,Pre,double>, typename DateT = DateTimes::Date>
struct Reading
{
	using Unit = U;
	using QuantityType = Q;
	using DateType = DateT;
	using Type = Reading<Unit,Pre,QuantityType,DateType>;
	const DateType Date;
	const QuantityType QuantityValue;
	
	template<typename Separator = T::char_<';'>>
	std::ostream& Display(std::ostream& out) const	{ return out<<Date<<Separator::Value<<QuantityValue.Value()<<Separator::Value<<QuantityType::UnitPrefix::Sign<<U::Sign()<<std::endl;	}
	Reading(QuantityType val, DateType d): Date(d), QuantityValue(val)	{}
	
	decltype(auto) operator+(const Type& left) {return Result<Addition,Type,Type,QuantityType>(*this,left, this->QuantityValue + left.QuantityValue);}
};

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator+(const Q& q, const Reading<U,P,Q,D>& r) { return q + r.QuantityValue;}

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator+(const Reading<U,P,Q,D>& r, const Q& q) { return q + r.QuantityValue;}

template<typename C,typename T = double, typename DateT = Date>
std::ostream& operator<<(std::ostream& strm, const Reading<C,T,DateT> c)	{	return c.Display(strm);}

using GasConfiguration = CounterConfiguration<Gas,1202757, Volume>;
using EnBWEnergyConfiguration = CounterConfiguration<Energy,21740069, Work>;
using VattenfallEnergyConfiguration = CounterConfiguration<Energy,11144078, Work>;
using AllWaterConfiguration = CounterConfiguration<Water,18061860, Volume>;
using OWaterConfiguration = CounterConfiguration<Water,279638, Volume>;
using OutWaterConfiguration = CounterConfiguration<Water,458711, Volume>;
using Bottom_HWaterConfiguration = CounterConfiguration<Water,15018324, Volume, AdditionalInformation<Hot>>;
using Bottom_CWaterConfiguration = CounterConfiguration<Water,15007241, Volume, AdditionalInformation<Cold>>;
using Middle_CWaterConfiguration = CounterConfiguration<Water,23267492, Volume, AdditionalInformation<Cold>>;
using Middle_HWaterConfiguration = CounterConfiguration<Water,14524889, Volume, AdditionalInformation<Hot>>;
using Top_CWaterConfiguration = CounterConfiguration<Water,25489823, Volume, AdditionalInformation<Cold>>;
using Top_HWaterConfiguration = CounterConfiguration<Water,25470737, Volume, AdditionalInformation<Hot>>;

#endif
