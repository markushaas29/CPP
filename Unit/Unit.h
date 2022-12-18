#include <string>
#include <iostream>
#include "../Logger/Logger.hpp" 

#pragma once

typedef int dimension[8];


template<typename Derived>
struct UnitTypeBase
{
	static std::string Unit() 
	{ 
		if(Derived::N == 0) return "";
		if(Derived::N == 1) return Derived::Sign;
		return Derived::Sign + "^(" + std::to_string(Derived::N) + ")"; };
};

template<int n = 0>
struct MassType: public UnitTypeBase<MassType<n>>
{
    enum{ N = n };
	using Type = MassType<N>;
    inline static const std::string Name = "Mass";
    inline static const std::string Symbol = "M";
    inline static const std::string Sign = "kg";
};

template<int n = 0>
struct LengthType: public UnitTypeBase<LengthType<n>>
{
    enum{ N = n };
	using Type = LengthType<N>;
    inline static const std::string Name = "Length";
    inline static const std::string Symbol = "L";
    inline static const std::string Sign = "m";
};

template<int n = 0>
struct TimeType: public UnitTypeBase<TimeType<n>>
{
    enum{ N = n };
	using Type = TimeType<N>;
    inline static const std::string Name = "Time";
    inline static const std::string Symbol = "T";
    inline static const std::string Sign = "s";
};

template<int n = 0>
struct CurrentType: public UnitTypeBase<CurrentType<n>>
{
    enum{ N = n };
	using Type = CurrentType<N>;
    inline static const std::string Name = "Current";
    inline static const std::string Symbol = "I";
    inline static const std::string Sign = "A";
};

template<int n = 0>
struct TemperatureType: public UnitTypeBase<TemperatureType<n>>
{
	enum{ N = n };
	using Type = TemperatureType<N>;
    inline static const std::string Name = "Temperature";
    inline static const std::string Symbol = "T";
    inline static const std::string Sign = "K";
};

template<int n = 0>
struct SubstanceAmount: public UnitTypeBase<SubstanceAmount<n>>
{
    enum{ N = n };
	using Type = SubstanceAmount<N>;
    inline static const std::string Name = "SubstanceAmount";
    inline static const std::string Symbol = "N";
    inline static const std::string Sign = "mol";
};

template<int n = 0>
struct LightIntensity: public UnitTypeBase<LightIntensity<n>>
{
    enum{ N = n };
	using Type = LightIntensity<N>;
    inline static const std::string Name = "LightIntensity";
    inline static const std::string Symbol = "Iv";
    inline static const std::string Sign = "cd";
};

template<int n = 0>
struct SumType: public UnitTypeBase<SumType<n>>
{
    enum{ N = n };
	using Type = SumType<N>;
    inline static const std::string Name = "Sum";
    inline static const std::string Symbol = "S";
    inline static const std::string Sign = "€";
};

template<typename U, int S = U::Sum::N, int L = U::Length::N, int M = U::Mass::N, int T = U::Time::N, int C = U::Current::N, int Te = U::Temperature::N, int Sub = U::SubstanceAmount::N, int Li = U::LightIntensity::N> 
struct UnitSign
{
	using UnitType = U;
	static std::string Get() { return UnitType::SiUnit(); }
};


template<int SumN = 0, int LengthN = 0, int MassN = 0, int TimeN = 0, int CurrentN = 0, int TemperatureN = 0, int SubstanceN = 0, int LightIntensityN = 0>
struct Unit
{
	using Mass = typename MassType<MassN>::Type;
	using Length = typename LengthType<LengthN>::Type;
	using Time = typename TimeType<TimeN>::Type;
	using Current = typename CurrentType<CurrentN>::Type;
	using Temperature = typename TemperatureType<TemperatureN>::Type;
	using SubstanceAmount = typename SubstanceAmount<SubstanceN>::Type;
	using LightIntensity = typename LightIntensity<LightIntensityN>::Type;
	using Sum = typename SumType<SumN>::Type;
	using Type = Unit<SumN, LengthN, MassN, TimeN, CurrentN, TemperatureN, SubstanceN, LightIntensityN>;
	
	static const std::string Name;
	static const std::string SiUnit() { return Mass::Unit() + Length::Unit() + Time::Unit() + Current::Unit() + Temperature::Unit() + SubstanceAmount::Unit() + LightIntensity::Unit() + Sum::Unit(); };
	static const std::string Sign() { return UnitSign<Type>::Get(); }; 
};

template<int a,int b,int c,int d,int e,int f,int g,int h> const std::string Unit<a,b,c,d,e,f,g,h>::Name = "unknown";

using Sum = Unit<1>;
using Length = Unit<0,1>;
using Mass = Unit<0,0,1>;
using Time = Unit<0,0,0,1>;
using Current = Unit<0,0,0,0,1>;
using Temperature = Unit<0,0,0,0,0,1>;
using Scalar = Unit<>;

using Volume = Unit<0,3>;
using Work = Unit<0,1,2,2>;
using Area = Unit<0,2>;

template<> const std::string Sum::Name = SumType<0>::Name;
template<> const std::string Current::Name = CurrentType<0>::Name;
template<> const std::string Scalar::Name = "Scalar";

template<> struct UnitSign<Work> {	static std::string Get(){ return "W";} };
// template<> struct UnitSign<Area> {	static std::string Get(){ return "m²";} };



template<class U1, class U2, template<typename, typename> class TransformPolicy>
struct Transform
{
	using MassT = typename MassType<TransformPolicy<typename U1::Mass, typename U2::Mass>::N>::Type;
	using LengthT = typename LengthType<TransformPolicy<typename U1::Length,typename U2::Length>::N>::Type;
	using TimeT = typename TimeType<TransformPolicy<typename U1::Time,typename U2::Time>::N>::Type;
	using CurrentT = typename CurrentType<TransformPolicy<typename U1::Current,typename U2::Current>::N>::Type;
	using TempT = typename TemperatureType<TransformPolicy<typename U1::Temperature,typename U2::Temperature>::N>::Type;
	using SubstanceT = typename SubstanceAmount<TransformPolicy<typename U1::SubstanceAmount,typename U2::SubstanceAmount>::N>::Type;
	using LightT = typename LightIntensity<TransformPolicy<typename U1::LightIntensity,typename U2::LightIntensity>::N>::Type;
	using SumT = typename SumType<TransformPolicy<typename U1::Sum,typename U2::Sum>::N>::Type;
	
	using Type = typename Unit<SumT::N, LengthT::N, MassT::N, TimeT::N, CurrentT::N, TempT::N, SubstanceT::N>::Type;
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

