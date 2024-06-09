#include <string>
#include <array>
#include <iostream>
#include "../CSV/TokenLiterals.hpp" 
#include "../Logger/Logger.hpp" 

#pragma once

typedef int dimension[8];


template<typename Derived>
struct UnitTypeBase
{
	static std::string Unit() 
	{ 
		if(Derived::N == 0) return "";
		if(Derived::N == 1) return std::string(Derived::Sign);
		return std::string(Derived::Sign) + "^" + std::to_string(Derived::N); };
};

template<int n = 0>
struct MassType: public UnitTypeBase<MassType<n>>
{
	inline static constexpr int N = n;
	using Type = MassType<N>;
    inline static constexpr const char* Name = "Mass";
    inline static constexpr const char* Symbol = "M";
    inline static constexpr const char* Sign = "kg";
};

template<int n = 0>
struct LengthType: public UnitTypeBase<LengthType<n>>
{
	inline static constexpr int N = n;
	using Type = LengthType<N>;
    inline static constexpr const char* Name = "Length";
    inline static constexpr const char* Symbol = "L";
    inline static constexpr const char* Sign = "m";
};

template<int n = 0>
struct TimeType: public UnitTypeBase<TimeType<n>>
{
	inline static constexpr int N = n;
	using Type = TimeType<N>;
    inline static constexpr const char* Name = "Time";
    inline static constexpr const char* Symbol = "T";
    inline static constexpr const char* Sign = "s";
};

template<int n = 0>
struct CurrentType: public UnitTypeBase<CurrentType<n>>
{
	inline static constexpr int N = n;
	using Type = CurrentType<N>;
    inline static constexpr const char* Name = "Current";
    inline static constexpr const char* Symbol = "I";
    inline static constexpr const char* Sign = "A";
};

template<int n = 0>
struct TemperatureType: public UnitTypeBase<TemperatureType<n>>
{
	inline static constexpr int N = n;
	using Type = TemperatureType<N>;
    inline static constexpr const char* Name = "Temperature";
    inline static constexpr const char* Symbol = "T";
    inline static constexpr const char* Sign = "K";
};

template<int n = 0>
struct AngleType: public UnitTypeBase<AngleType<n>>
{
	inline static constexpr int N = n;
	using Type = AngleType<N>;
    inline static constexpr const char* Name = "AngleType";
    inline static constexpr const char* Symbol = "N";
    inline static constexpr const char* Sign = "mol";
};

template<int n = 0>
struct IntensityType: public UnitTypeBase<IntensityType<n>>
{
	inline static constexpr int N = n;
	using Type = IntensityType<N>;
    inline static constexpr const char* Name = "IntensityType";
    inline static constexpr const char* Symbol = "Iv";
    inline static constexpr const char* Sign = "cd";
};

template<int n = 0>
struct SumType: public UnitTypeBase<SumType<n>>
{
	inline static constexpr int N = n;
	using Type = SumType<N>;
    inline static constexpr const char* Name = "Sum";
    inline static constexpr const char* Symbol = "S";
    inline static constexpr const char* Sign = "€";
};

template<typename U, int S = U::Sum::N, int L = U::Length::N, int M = U::Mass::N, int T = U::Time::N, int C = U::Current::N, int Te = U::Temperature::N, int Sub = U::AngleType::N, int Li = U::IntensityType::N> 
struct UnitSign
{
	using UnitType = U;
	static std::string Get() { return UnitType::SiUnit(); }
};


template<int SumN = 0, int LengthN = 0, int MassN = 0, int TimeN = 0, int CurrentN = 0, int TemperatureN = 0, int AngleN = 0, int IntensityTypeN = 0>
struct Unit
{
	using Mass = typename MassType<MassN>::Type;
	using Length = typename LengthType<LengthN>::Type;
	using Time = typename TimeType<TimeN>::Type;
	using Current = typename CurrentType<CurrentN>::Type;
	using Temperature = typename TemperatureType<TemperatureN>::Type;
	using AngleType = typename AngleType<AngleN>::Type;
	using IntensityType = typename IntensityType<IntensityTypeN>::Type;
	using Sum = typename SumType<SumN>::Type;
	using Type = Unit<SumN, LengthN, MassN, TimeN, CurrentN, TemperatureN, AngleN, IntensityTypeN>;
	
	static Unit& Instance()
	{
		static Unit instance;
		SiUnit();
		return instance;
	}
	
	static const char* Name;
	inline static const std::string TokenName = std::string(Name) + TokenIdentifier::TypeIdentifier;
	static const std::string SiUnit() { return Mass::Unit() + std::string(Length::Unit()) + std::string(Time::Unit()) + std::string(Current::Unit()) + Temperature::Unit() + AngleType::Unit() + IntensityType::Unit() + Sum::Unit(); };
	static const std::string Sign() { return UnitSign<Type>::Get(); }; 
};

template<typename Si1, typename Si2 = Si1>
constexpr double CalculateFactor()
{ 
	if(Si1::N == 0 && Si2::N == 0)
		return -1.0;
	
	return  Si2::N == 0.0 ? 0.0 : ((double)Si1::N)/((double)Si2::N);
}

template<typename U1, typename U2>
constexpr bool IsSameBaseUnit()
{ 
	auto arr = std::array<double,7>();
	
	arr[0] = CalculateFactor<typename U1::Mass,typename U2::Mass>(); 
	arr[1] = CalculateFactor<typename U1::Length,typename U2::Length>(); 
	arr[2] = CalculateFactor<typename U1::Time,typename U2::Time>(); 
	arr[3] = CalculateFactor<typename U1::Current,typename U2::Current>(); 
	arr[4] = CalculateFactor<typename U1::Temperature,typename U2::Temperature>(); 
	arr[5] = CalculateFactor<typename U1::AngleType,typename U2::AngleType>(); 
	arr[5] = CalculateFactor<typename U1::IntensityType,typename U2::IntensityType>(); 
	arr[6] = CalculateFactor<typename U1::Sum,typename U2::Sum>(); 

	auto zeros =  std::count(arr.cbegin(), arr.cend(),0.0);
	auto negatives =  std::count(arr.cbegin(), arr.cend(),-1.0);
	
	if(zeros > 1)
		return false;

	std::sort(arr.begin(), arr.end());
	auto uniqueCount = std::unique(arr.begin(), arr.end()) - arr.begin();
	if(uniqueCount <= 1)
		return true;
	return zeros == 1 && negatives > 0 && uniqueCount > 3 ? false : true; 
}

template<int a,int b,int c,int d,int e,int f,int g,int h> const char* Unit<a,b,c,d,e,f,g,h>::Name = "unknown";

using Sum = Unit<1>;
using Length = Unit<0,1>;
using Mass = Unit<0,0,1>;
using Time = Unit<0,0,0,1>;
using Current = Unit<0,0,0,0,1>;
using Temperature = Unit<0,0,0,0,0,1>;
using Scalar = Unit<>;

using Volume = Unit<0,3>;
using Energy = Unit<0,1,2,-3>;
using Area = Unit<0,2>;
using SumPerArea = Unit<1,-2>;

template<> const char* Sum::Name = SumType<0>::Name;
template<> const char* Energy::Name = "Work";
template<> const char* Current::Name = CurrentType<0>::Name;
template<> const char* Scalar::Name = "Scalar";
template<> const char* Length::Name = "Length";
template<> const char* Volume::Name = "Volume";
template<> const char* Area::Name = "Area";
template<> const char* SumPerArea::Name = "SumPerArea";

template<> struct UnitSign<Energy> {	static constexpr const char* Get(){ return "Ws";} };
// template<> struct UnitSign<Area> {	static std::string Get(){ return "m²";} };



template<class U1, class U2, template<typename, typename> class TransformPolicy>
struct Transform
{
	using MassT = typename MassType<TransformPolicy<typename U1::Mass, typename U2::Mass>::N>::Type;
	using LengthT = typename LengthType<TransformPolicy<typename U1::Length,typename U2::Length>::N>::Type;
	using TimeT = typename TimeType<TransformPolicy<typename U1::Time,typename U2::Time>::N>::Type;
	using CurrentT = typename CurrentType<TransformPolicy<typename U1::Current,typename U2::Current>::N>::Type;
	using TempT = typename TemperatureType<TransformPolicy<typename U1::Temperature,typename U2::Temperature>::N>::Type;
	using AngleT = typename AngleType<TransformPolicy<typename U1::AngleType,typename U2::AngleType>::N>::Type;
	using IntensityT = typename IntensityType<TransformPolicy<typename U1::IntensityType,typename U2::IntensityType>::N>::Type;
	using SumT = typename SumType<TransformPolicy<typename U1::Sum,typename U2::Sum>::N>::Type;
	
	using Type = typename Unit<SumT::N, LengthT::N, MassT::N, TimeT::N, CurrentT::N, TempT::N, AngleT::N>::Type;
};

template<class D1, class D2>
struct MultiplyPolicy
{
	static constexpr int N = D1::N + D2::N;
};

template<class D1, class D2>
struct DividePolicy
{
	static constexpr int N = D1::N - D2::N;
};

