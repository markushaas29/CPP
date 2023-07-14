#include <memory>
#include <tuple>
#include <vector>
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#pragma once

template<size_t, typename, typename> class MatrixDescriptor;
template<size_t, typename> class Matrix;

template<typename L, typename R>
struct Add
{
	using Type = decltype(std::declval<L>() + std::declval<R>());
};

template<template<typename,typename> class Op, typename M1, typename M2>
class MatrixOperation 
{
public:
	using Left = M1;
	using Right = M2;
	using ET = Op<typename Left::InpuType, typename Right::InputType>::Type;
	using DET = std::shared_ptr<ET>;    
	using MDET = MatrixDescriptor<Left::Order,ET,ET>;
	using MET = Matrix<Left::Order,MDET>;
	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = Left::Order;
protected:
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixOperation& me) { return s;  }
};

