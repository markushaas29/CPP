#include <memory>
#include <tuple>
#include <vector>
#include <type_traits>
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

template<size_t, typename, typename, typename> class MatrixDescriptor;
template<size_t, typename> class Matrix;

template<typename T, typename L, typename R>
class OperationBase 
{
	using LeftType = L;
	using RightType = R;
	using Type = T;
public:
	OperationBase(const RightType& v): val{v} {}
	decltype(auto) operator()(const auto& v) { return v + val; }
private:
	RightType val;
};

template<typename L, typename R>
class Add: public OperationBase<Addition,L,R>
{
	using Base = OperationBase<Addition,L,R>;
public:
	Add(const L& v): Base{v} {}
	using ExpressionType = decltype(Addition::Calculate(std::declval<L>(), std::declval<R>()));
	using Type = decltype(std::declval<L>() + std::declval<R>());
};

template<typename L, typename R>
class Sub: public OperationBase<Subtraction,L,R>
{
	using Base = OperationBase<Subtraction,L,R>;
public:
	Sub(const L& v): Base{v} {}
	using ExpressionType = decltype(Subtraction::Calculate(std::declval<L>(), std::declval<R>()));
	using Type = decltype(std::declval<L>() - std::declval<R>());
};

template<typename L, typename R>
class Mul: public OperationBase<Multiplication,L,R>
{
	using Base = OperationBase<Multiplication,L,R>;
public:
	Mul(const L& v): Base{v} {}
	using ExpressionType = decltype(Multiplication::Calculate(std::declval<L>(), std::declval<R>()));
	using Type = decltype(std::declval<L>() * std::declval<R>());
};

template<typename L, typename R>
class Div: public OperationBase<Division,L,R>
{
	using Base = OperationBase<Division,L,R>;
public:
	Div(const L& v): Base{v} {}
	using ExpressionType = decltype(Division::Calculate(std::declval<L>(), std::declval<R>()));
	using Type = decltype(std::declval<L>() / std::declval<R>());
};

template<typename Op, typename M1, typename V>
class ValueOperation 
{
public:
	using Left = M1;
	using Right = V;
	using ValueType = Op::Type;
	using ExpressionType = Op::ExpressionType;
	using DataType = std::shared_ptr<ValueType>;    
	using ExpDataType = std::shared_ptr<ExpressionType>;    
	using DescriptorType = MatrixDescriptor<Left::Order,ValueType,ExpressionType>;
	using MatrixType = Matrix<Left::Order,DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = Left::Order;
protected:
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const ValueOperation& me) { return s;  }
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

