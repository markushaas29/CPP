#include <memory>
#include <tuple>
#include <vector>
#include <type_traits>
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Calculator/Operations.hpp"
#pragma once

template<size_t, typename> class MatrixDescriptor;
template<size_t, typename> class Matrix;

template<typename T, template<typename,typename> class D,typename L, typename R>
class OperationBase 
{
	using LeftType = L;
	using RightType = R;
	using Type = T;
	using OpType = D<L,R>;
public:
	OperationBase(const RightType& v): val{v} {}
	decltype(auto) operator()(const auto& v) { return T::Calculate(v,val); }
private:
	RightType val;
};

template<typename L, typename R>
class Add: public OperationBase<Addition,Add,L,R>
{
	using Base = OperationBase<Addition,Add,L,R>;
	friend class OperationBase<Addition,Add,L,R>;
public:
	Add(const L& v): Base{v} {}
	using Type = decltype(Addition::Calculate(std::declval<L>(), std::declval<R>()));
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v + val; }
};

template<typename L, typename R>
class Sub: public OperationBase<Subtraction,Sub,L,R>
{
	using Base = OperationBase<Subtraction,Sub,L,R>;
	friend class OperationBase<Subtraction,Sub,L,R>;
public:
	Sub(const L& v): Base{v} {}
	using Type = decltype(Subtraction::Calculate(std::declval<L>(), std::declval<R>()));
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v - val; }
};

template<typename L, typename R>
class Mul: public OperationBase<Multiplication,Mul,L,R>
{
	using Base = OperationBase<Multiplication,Mul,L,R>;
	friend class OperationBase<Multiplication,Mul,L,R>;
public:
	Mul(const L& v): Base{v} {}
	using Type = decltype(Multiplication::Calculate(std::declval<L>(), std::declval<R>()));
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v * val; }
};

template<typename L, typename R>
class Div: public OperationBase<Division,Div,L,R>
{
	using Base = OperationBase<Division,Div,L,R>;
	friend class OperationBase<Division,Div,L,R>;
public:
	Div(const L& v): Base{v} {}
	using Type = decltype(Division::Calculate(std::declval<L>(), std::declval<R>()));
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v / val; }
};

template<typename Op, typename M1, typename V>
class ValueOperation 
{
public:
	using Left = M1;
	using Right = V;
	using ValueType = Op::Type;
	using DataType = std::shared_ptr<ValueType>;    
	using DescriptorType = MatrixDescriptor<Left::Order,ValueType>;
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
	using ValueType = Op<typename Left::ElementType, typename Right::ElementType>::Type;
	using DataType = std::shared_ptr<ValueType>;    
	using DescriptorType = MatrixDescriptor<Left::Order,ValueType>;
	using MatrixType = Matrix<Left::Order,DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = Left::Order;
protected:
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixOperation& me) { return s;  }
};

