#include <memory>
#include <ratio>
#include "../Math/Math.hpp"
#include "../Unit/Unit.hpp"

#pragma once

//~ template<int a,int b,int c,int d,int e,int f,int g,int h> class Unit;

template<uint N, uint D, int Ex, template<int> class Derived>
struct QuantityRatioBase
{
	using Type = Derived<Ex>;
	
	static constexpr int Exponent = Ex;
	static constexpr uint Num = N;
	static constexpr uint Denom = D;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
	
	template<int Factor>
	struct MultiplyBy
	{
		static constexpr int T = Factor * Num;
	};
};

//~ template<uint N, uint D, int U>
//~ struct QuantityRatio: public QuantityRatioBase<1000,1,N>
//~ {
	//~ using Base = QuantityRatioBase<1000,1,N>;
	//~ template<int E> using Creator = QuantityRatio<E,U>;
//~ };

//~ template<int N>
//~ struct QuantityRatio<N,Unit<0,0,0,1,0,0,0,0>>: public QuantityRatioBase<60,1,N>
//~ {
	//~ using Base = QuantityRatioBase<60,1,N>;
//~ };

template<int Ex>
struct MinutesBase: public QuantityRatioBase<60, 1, Ex, MinutesBase> 
{	
	//~ template<int E> using Creator = Minutes<E>;
	inline static constexpr const char* Sign = "min"; 
	inline static constexpr const char* Name = "Minuttes"; 
};

template<int Ex>
struct HoursBase: public QuantityRatioBase<3600, 1,Ex, HoursBase> 
{	
	inline static constexpr const char* Sign = "h"; 
	inline static constexpr const char* Name = "Hours"; 
};

template<int Ex>
struct DaysBase: public QuantityRatioBase<86400, 1, Ex,DaysBase> 
{	
	inline static constexpr const char* Sign = "d"; 
	inline static constexpr const char* Name = "Days"; 
};

template<int Ex>
struct MilliBase: public QuantityRatioBase<1, 1000, Ex, MilliBase> 
{	
	inline static constexpr const char* Sign = "m"; 
	inline static constexpr const char* Name = "Milli"; 
};

template<int Ex>
struct PureBase: public QuantityRatioBase<1,1,Ex,PureBase> 
{	
	inline static constexpr const char* Sign = ""; 
	inline static constexpr const char* Name = "Pure"; 
};

template<int Ex>
struct KiloBase: public QuantityRatioBase<1000, 1, Ex, KiloBase> 
{	
	inline static constexpr const char* Sign = "k"; 
	inline static constexpr const char* Name = "Kilo"; 
};

//~ template<int Ex>
//~ struct Hekto: public SIPrefix<100, 1> 
//~ {	
	//~ inline static const std::string Sign = "h"; 
	//~ inline static const std::string Name = "Hekto"; 
//~ };

template<int Ex>
struct DekaBase: public QuantityRatioBase<10,1,Ex,DekaBase> 
{	
	inline static constexpr const char* Sign = "da"; 
	inline static constexpr const char* Name = "Deka"; 
};

using Minutes = MinutesBase<1>;
using Hours = HoursBase<1>;
using Days = DaysBase<1>;
using Milli = MilliBase<1>;
using Kilo = KiloBase<1>;
using Pure = PureBase<1>;
using Deka = DekaBase<1>;
