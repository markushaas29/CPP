#include <string>
#include <array>
#include <iostream>
#include "Unit.hpp" 
#include "../CSV/TokenLiterals.hpp" 
#include "../Logger/Logger.hpp" 
#include "../Quantity/QuantityRatio.hpp" 

#pragma once

typedef int dimension[8];


template<int minN = 0, int hourN = 0, int dayN = 0, int inchN = 0, int mileN = 0,int TemperatureN = 0, int AngleN = 0, int IntensityTypeN = 0>
struct UnitRatio
{
	using Min = typename MinutesBase<minN>::Type;
	using Hours = typename HoursBase<hourN>::Type;
	using Days = typename DaysBase<dayN>::Type;
	using Inch = typename InchBase<inchN>::Type;
	using Miles = typename MileBase<mileN>::Type;
	using Type = UnitRatio<minN, hourN, dayN, inchN, mileN, TemperatureN, AngleN, IntensityTypeN>;
	
	static UnitRatio& Instance()
	{
		static UnitRatio instance;
		SiUnitRatio();
		return instance;
	}
	
	static const char* Name;
	inline static const std::string TokenName = std::string(Name) + TokenIdentifier::TypeIdentifier;
	static const std::string SiUnitRatio() { return Min::UnitRatio() + std::string(Hours::UnitRatio()) + std::string(Days::UnitRatio()) + std::string(Inch::UnitRatio()) + Miles::UnitRatio(); };
};

template<class U1, class U2, template<typename, typename> class TransformPolicy>
struct TransformRatio
{
	using MinT = typename MinutesBase<TransformPolicy<typename U1::Min, typename U2::Min>::N>::Type;
	using HoursT = typename HoursBase<TransformPolicy<typename U1::Hours,typename U2::Hours>::N>::Type;
	using DaysT = typename DaysBase<TransformPolicy<typename U1::Days,typename U2::Days>::N>::Type;
	using InchT = typename InchBase<TransformPolicy<typename U1::Inch,typename U2::Inch>::N>::Type;
	using MilesT = typename MileBase<TransformPolicy<typename U1::Miles,typename U2::Miles>::N>::Type;
	
	using Type = typename Unit<MinT::N, HoursT::N, DaysT::N, InchT::N, MilesT::N>::Type;
};
//
//template<class D1, class D2>
//struct MultiplyPolicy
//{
//	static constexpr int N = D1::N + D2::N;
//};
//
//template<class D1, class D2>
//struct DividePolicy
//{
//	static constexpr int N = D1::N - D2::N;
//};

