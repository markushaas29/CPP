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
	static constexpr uint Num = Ex > 0 ? Math::Pow<N,Ex>::Result : 1;
	static constexpr uint Denom = Ex < 0 ? Math::Pow<N,-Ex>::Result : 1;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
};

template<int N, typename U>
struct QuantityRatio: public QuantityRatioBase<1000,1,N>
{
	using Base = QuantityRatioBase<1000,1,N>;
	template<int E> using Creator = QuantityRatio<E,U>;
};

template<int N>
struct QuantityRatio<N,Unit<0,0,0,1,0,0,0,0>>: public QuantityRatioBase<60,1,N>
{
	using Base = QuantityRatioBase<60,1,N>;
};



//~ struct Milli: public SIPrefix<1, 1000> 
//~ {	
	//~ inline static const std::string Sign = "mil"; 
	//~ inline static const std::string Name = "Milli"; 
//~ };

template<int N = 1>
struct Minutes: public QuantityRatio<N,Time> 
{	
	template<int E> using Creator = Minutes<E>;
	inline static constexpr const char* Sign = "min"; 
	inline static constexpr const char* Name = "Minuttes"; 
};

//~ struct Hours: public SIPrefix<3600, 1> 
//~ {	
	//~ inline static const std::string Sign = "h"; 
	//~ inline static const std::string Name = "Hours"; 
//~ };

//~ struct Days: public SIPrefix<86400, 1> 
//~ {	
	//~ inline static const std::string Sign = "d"; 
	//~ inline static const std::string Name = "Days"; 
//~ };

template<typename U>
struct Pure: public QuantityRatio<0, U> 
{	
	inline static constexpr const char* Sign = ""; 
	inline static constexpr const char* Name = ""; 
};

//~ struct Kilo: public SIPrefix<1000, 1> 
//~ {	
	//~ inline static const std::string Sign = "k"; 
	//~ inline static const std::string Name = "Kilo"; 
//~ };

//~ struct Hekto: public SIPrefix<100, 1> 
//~ {	
	//~ inline static const std::string Sign = "h"; 
	//~ inline static const std::string Name = "Hekto"; 
//~ };

//~ struct Deka: public SIPrefix<10, 1> 
//~ {	
	//~ inline static const std::string Sign = "da"; 
	//~ inline static const std::string Name = "Deka";
//~ };
