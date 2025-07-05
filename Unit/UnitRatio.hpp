#include <string>
#include <array>
#include <iostream>
#include "Unit.hpp" 
#include "../CSV/TokenLiterals.hpp" 
#include "../Logger/Logger.hpp" 
#include "../Quantity/QuantityRatio.hpp" 

#pragma once

typedef int dimension[8];


template<int SumN = 0, int LengthN = 0, int MassN = 0, int TimeN = 0, int CurrentN = 0, int TemperatureN = 0, int AngleN = 0, int IntensityTypeN = 0>
struct UnitRatio
{
	using Mass = typename MassType<MassN>::Type;
	using Length = typename LengthType<LengthN>::Type;
	using Time = typename TimeType<TimeN>::Type;
	using Current = typename CurrentType<CurrentN>::Type;
	using Temperature = typename TemperatureType<TemperatureN>::Type;
	using AngleType = typename AngleType<AngleN>::Type;
	using IntensityType = typename IntensityType<IntensityTypeN>::Type;
	using Sum = typename SumType<SumN>::Type;
	using Type = UnitRatio<SumN, LengthN, MassN, TimeN, CurrentN, TemperatureN, AngleN, IntensityTypeN>;
	
	static UnitRatio& Instance()
	{
		static UnitRatio instance;
		SiUnitRatio();
		return instance;
	}
	
	static const char* Name;
	inline static const std::string TokenName = std::string(Name) + TokenIdentifier::TypeIdentifier;
	static const std::string SiUnitRatio() { return Mass::UnitRatio() + std::string(Length::UnitRatio()) + std::string(Time::UnitRatio()) + std::string(Current::UnitRatio()) + Temperature::UnitRatio() + AngleType::UnitRatio() + IntensityType::UnitRatio() + Sum::UnitRatio(); };
};

//template<class U1, class U2, template<typename, typename> class TransformPolicy>
//struct Transform
//{
//	using MassT = typename MassType<TransformPolicy<typename U1::Mass, typename U2::Mass>::N>::Type;
//	using LengthT = typename LengthType<TransformPolicy<typename U1::Length,typename U2::Length>::N>::Type;
//	using TimeT = typename TimeType<TransformPolicy<typename U1::Time,typename U2::Time>::N>::Type;
//	using CurrentT = typename CurrentType<TransformPolicy<typename U1::Current,typename U2::Current>::N>::Type;
//	using TempT = typename TemperatureType<TransformPolicy<typename U1::Temperature,typename U2::Temperature>::N>::Type;
//	using AngleT = typename AngleType<TransformPolicy<typename U1::AngleType,typename U2::AngleType>::N>::Type;
//	using IntensityT = typename IntensityType<TransformPolicy<typename U1::IntensityType,typename U2::IntensityType>::N>::Type;
//	using SumT = typename SumType<TransformPolicy<typename U1::Sum,typename U2::Sum>::N>::Type;
//	
//	using Type = typename Unit<SumT::N, LengthT::N, MassT::N, TimeT::N, CurrentT::N, TempT::N, AngleT::N>::Type;
//};
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

