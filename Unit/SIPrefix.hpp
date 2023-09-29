#include <memory>
#include <ratio>
#include "../Math/Math.hpp"

#ifndef SIPREFIX_HPP
#define SIPREFIX_HPP

template<uint N, uint D>
struct SIPrefix
{
	using Type = SIPrefix<N,D>;
	
	static constexpr uint Num = N;
	static constexpr uint Denom = D;
	static constexpr std::ratio<Num, Denom> Ratio = std::ratio<Num, Denom>();
	static inline const std::string Sign;
	static inline const std::string Name;
	static constexpr double Factor = ((double)Num / Denom);	
};


// template<>
// struct SIPrefix<2> 
// {	
// 	inline static const std::string Sign = "h"; 
// 	inline static const std::string Name = "Hekto"; 
// };

struct Milli: public SIPrefix<1, 1000> 
{	
	inline static const std::string Sign = "mil"; 
	inline static const std::string Name = "Milli"; 
};

struct Minutes: public SIPrefix<60, 1> 
{	
	inline static const std::string Sign = "min"; 
	inline static const std::string Name = "Minuttes"; 
};

struct Hours: public SIPrefix<3600, 1> 
{	
	inline static const std::string Sign = "h"; 
	inline static const std::string Name = "Hours"; 
};

struct Days: public SIPrefix<86400, 1> 
{	
	inline static const std::string Sign = "d"; 
	inline static const std::string Name = "Days"; 
};

struct Pure: public SIPrefix<1, 1> 
{	
	inline static const std::string Sign = ""; 
	inline static const std::string Name = ""; 
};

struct Kilo: public SIPrefix<1000, 1> 
{	
	inline static const std::string Sign = "k"; 
	inline static const std::string Name = "Kilo"; 
};

struct Hekto: public SIPrefix<100, 1> 
{	
	inline static const std::string Sign = "h"; 
	inline static const std::string Name = "Hekto"; 
};

struct Deka: public SIPrefix<10, 1> 
{	
	inline static const std::string Sign = "da"; 
	inline static const std::string Name = "Deka";
};

#endif
