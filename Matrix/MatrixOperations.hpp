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
#include "../Calculator/Operations.hpp"
#pragma once

template<size_t, typename, typename> class MatrixDescriptor;
template<size_t, typename> class Matrix;

template<typename L, typename R>
struct Add
{
	using Type = decltype(Addition::Calculate(std::declval<L>(), std::declval<R>()));
};

template<typename L, typename R>
struct Sub
{
	using Type = decltype(Subtraction::Calculate(std::declval<L>(), std::declval<R>()));
};

template<typename L, typename F>
struct Func
{
	using Type = decltype(Subtraction::Calculate(std::declval<L>(), std::declval<F>()));
};

template<template<typename,typename> class Op, typename M1, typename M2>
class MatrixOperation 
{
public:
	using Left = M1;
	using Right = std::remove_reference<M2>::type;
	using ExpressionType = Op<typename Left::InputType, typename Right::InputType>::Type;
	using DataType = std::shared_ptr<ExpressionType>;    
	using DescriptorType = MatrixDescriptor<Left::Order,ExpressionType,ExpressionType>;
	using MatrixType = Matrix<Left::Order,DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = Left::Order;
protected:
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixOperation& me) { return s;  }
};

template<typename M1, typename M2>
class MatrixOperation<Func,M1,M2> 
{
public:
	using Left = M1;
	using Right = std::remove_reference<M2>::type;
	using ExpressionType = Func<typename Left::InputType, M2>::Type;
	using DataType = std::shared_ptr<ExpressionType>;    
	using DescriptorType = MatrixDescriptor<Left::Order,ExpressionType,ExpressionType>;
	using MatrixType = Matrix<Left::Order,DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = Left::Order;
protected:
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixOperation& me) { return s;  }
};

