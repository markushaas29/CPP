#include <string>
#include <array>
#include <tuple>
#include <iostream>
#include "Unit.hpp" 
#include "../CSV/TokenLiterals.hpp" 
#include "../Logger/Logger.hpp" 
#include "../Quantity/QuantityRatio.hpp" 

#pragma once

typedef int dimension[8];

template<uint I, typename T>
struct TransformUnit
{
	using Type = typename Transform<typename std::tuple_element<I, T>::type, typename TransformUnit<I-1,T>::Type, MultiplyPolicy>::Type;
};

template<typename T>
struct TransformUnit<1,T>
{
	using Type = typename Transform<typename std::tuple_element<0, T>::type, typename std::tuple_element<1, T>::type, MultiplyPolicy>::Type;
};

template<int minN = 0, int hourN = 0, int dayN = 0, int inchN = 0, int mileN = 0,int literN = 0, int AngleN = 0, int IntensityTypeN = 0>
struct UnitRatio
{
	using Min = typename MinutesBase<minN>::Type;
	using Hours = typename HoursBase<hourN>::Type;
	using Days = typename DaysBase<dayN>::Type;
	using Inch = typename InchBase<inchN>::Type;
	using Miles = typename MileBase<mileN>::Type;
	using Liter = typename LiterBase<literN>::Type;
	using Ratios = std::tuple<Min,Hours,Days,Inch,Miles,Liter>;
	using UnitRatioType = UnitRatio<minN, hourN, dayN, inchN, mileN, literN, AngleN, IntensityTypeN>;
	
	static UnitRatio& Instance()
	{
		static UnitRatio instance;
		return instance;
	}
private:
	inline static constexpr double factor()
	{
		double result = 1;
		auto tup = Ratios();
		std::apply([&result](auto... args) {(( result *= decltype(args)::Factor), ...);}, Ratios());
		return result;
	}

	template<class U1, class U2, template<typename, typename> class TransformPolicy>
	struct TransformRatio
	{
		using MinT = typename MinutesBase<TransformPolicy<typename U1::Min, typename U2::Min>::N>::Type;
		using HoursT = typename HoursBase<TransformPolicy<typename U1::Hours,typename U2::Hours>::N>::Type;
		using DaysT = typename DaysBase<TransformPolicy<typename U1::Days,typename U2::Days>::N>::Type;
		using InchT = typename InchBase<TransformPolicy<typename U1::Inch,typename U2::Inch>::N>::Type;
		using MilesT = typename MileBase<TransformPolicy<typename U1::Miles,typename U2::Miles>::N>::Type;
		using LiterT = typename LiterBase<TransformPolicy<typename U1::Liter,typename U2::Liter>::N>::Type;
		
		using Type = typename UnitRatio<MinT::N, HoursT::N, DaysT::N, InchT::N, MilesT::N, LiterT::N>::UnitRatioType;
		using Types = std::tuple<typename MinT::Unit,typename HoursT::Unit,typename DaysT::Unit,typename InchT::Unit,typename MilesT::Unit,typename LiterT::Unit>;
		inline static constexpr uint Size = std::tuple_size_v<Types>;
		using ResultingUnit = TransformUnit<Size-1, Types>::Type;
	};
public:
	using ResultingUnit = TransformRatio<UnitRatioType,UnitRatio<0>,MultiplyPolicy>::ResultingUnit;
	static constexpr double Factor = factor();
	inline static constexpr int N = 1;
	static const char* Name;
	static inline const std::string Sign = Min::UnitRatio() + std::string(Hours::UnitRatio()) + std::string(Days::UnitRatio()) + std::string(Inch::UnitRatio()) + Miles::UnitRatio() + Liter::UnitRatio();
	inline static const std::string TokenName = std::string(Name) + TokenIdentifier::TypeIdentifier;
	static const std::string Unit() { return Min::UnitRatio() + std::string(Hours::UnitRatio()) + std::string(Days::UnitRatio()) + std::string(Inch::UnitRatio()) + Miles::UnitRatio() + Liter::UnitRatio(); };

	template<int Fac>
	struct PowBy
	{
		static constexpr int Factor = Fac;
		using Type = UnitRatioType;
	};
	
	template<typename U2>
	static auto TransformUnit() { return Sign + Transform<ResultingUnit, U2, MultiplyPolicy>::Type::Sign();; };
};

template<class U1, class U2, template<typename, typename> class TransformPolicy>
struct TransformRatio
{
	using MinT = typename MinutesBase<TransformPolicy<typename U1::Min, typename U2::Min>::N>::Type;
	using HoursT = typename HoursBase<TransformPolicy<typename U1::Hours,typename U2::Hours>::N>::Type;
	using DaysT = typename DaysBase<TransformPolicy<typename U1::Days,typename U2::Days>::N>::Type;
	using InchT = typename InchBase<TransformPolicy<typename U1::Inch,typename U2::Inch>::N>::Type;
	using MilesT = typename MileBase<TransformPolicy<typename U1::Miles,typename U2::Miles>::N>::Type;
	using LiterT = typename LiterBase<TransformPolicy<typename U1::Liter,typename U2::Liter>::N>::Type;
	
	using Type = typename UnitRatio<MinT::N, HoursT::N, DaysT::N, InchT::N, MilesT::N, LiterT::N>::UnitRatioType;
	using Types = std::tuple<typename MinT::Unit,typename HoursT::Unit,typename DaysT::Unit,typename InchT::Unit,typename MilesT::Unit,typename LiterT::Unit>;
	inline static constexpr uint Size = std::tuple_size_v<Types>;
	using ResultingUnit = TransformUnit<Size-1, Types>::Type;
};
