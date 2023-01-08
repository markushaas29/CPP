#include <memory>
#include <ratio>
#include "../Math/Math.hpp"
#include "../Unit/Unit.hpp"

#pragma once

//~ template<int a,int b,int c,int d,int e,int f,int g,int h> class Unit;

template<uint N, uint D, int Ex>
struct QuantityRatioBase
{
	using Type = QuantityRatioBase<N,D,Ex>;
	
	static constexpr int Exponent = Ex;
	static constexpr uint Num = N;
	static constexpr uint Denom = D;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
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

struct Minutes: public QuantityRatioBase<60, 1, 1> 
{	
	//~ template<int E> using Creator = Minutes<E>;
	inline static constexpr const char* Sign = "min"; 
	inline static constexpr const char* Name = "Minuttes"; 
};

struct Hours: public QuantityRatioBase<3600, 1,1> 
{	
	inline static constexpr const char* Sign = "h"; 
	inline static constexpr const char* Name = "Hours"; 
};

struct Days: public QuantityRatioBase<86400, 1, 1> 
{	
	inline static constexpr const char* Sign = "d"; 
	inline static constexpr const char* Name = "Days"; 
};

struct Milli: public QuantityRatioBase<1, 1000, -3> 
{	
	inline static constexpr const char* Sign = "m"; 
	inline static constexpr const char* Name = "Milli"; 
};

struct Pure: public QuantityRatioBase<1,1,0> 
{	
	inline static constexpr const char* Sign = ""; 
	inline static constexpr const char* Name = "Pure"; 
};

struct Kilo: public QuantityRatioBase<1000, 1, 3> 
{	
	inline static constexpr const char* Sign = "k"; 
	inline static constexpr const char* Name = "Kilo"; 
};

//~ struct Hekto: public SIPrefix<100, 1> 
//~ {	
	//~ inline static const std::string Sign = "h"; 
	//~ inline static const std::string Name = "Hekto"; 
//~ };

struct Deka: public QuantityRatioBase<10,1,1> 
{	
	inline static constexpr const char* Sign = "da"; 
	inline static constexpr const char* Name = "Deka"; 
};
