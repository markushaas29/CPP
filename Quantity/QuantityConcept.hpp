#include "../To/To.hpp"
#pragma once

template<typename T>
concept QuantityConcept = requires(T t)
{
	T::ValueType;
	T::UnitType;
	T::PureType;
	T::Calculator;
	T::QuantityRatioType;

	t.Value();
	t.PureValue();
};
