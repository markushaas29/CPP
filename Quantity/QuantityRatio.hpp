#include <memory>
#include <ratio>

#pragma once

template<int a,int b,int c,int d,int e,int f,int g,int h> class Unit;

template<uint N, uint D>
struct QuantityRatioBase
{
	using Type = QuantityRatioBase<N,D>;
	
	static constexpr uint Num = N;
	static constexpr uint Denom = D;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
};

template<int N, typename U>
struct QuantityRatio: public QuantityRatioBase<1000,1>
{
	using Base = QuantityRatioBase<1000,1>;
};

template<int N>
struct QuantityRatio<N,Unit<0,0,0,N,0,0,0,0>>: public QuantityRatioBase<60,1>
{
	using Base = QuantityRatioBase<60,1>;
};



//~ struct Milli: public SIPrefix<1, 1000> 
//~ {	
	//~ inline static const std::string Sign = "mil"; 
	//~ inline static const std::string Name = "Milli"; 
//~ };

//~ struct Minutes: public SIPrefix<60, 1> 
//~ {	
	//~ inline static const std::string Sign = "min"; 
	//~ inline static const std::string Name = "Minuttes"; 
//~ };

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

//~ struct Pure: public SIPrefix<1, 1> 
//~ {	
	//~ inline static const std::string Sign = ""; 
	//~ inline static const std::string Name = ""; 
//~ };

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
