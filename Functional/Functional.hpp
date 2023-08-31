#include <memory>
#include <tuple>
#include <vector>
#include <type_traits>
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Calculator/Operations.hpp"
#pragma once

template<class Domain, class Range>
class Functional
{
public:
	virtual Range operator()(const Domain&) const = 0;
	virtual ~Functional(){};
};

template<class Domain=double>
class Constant
{
public:
	Constant(const Domain& v): val{v} {}
	virtual Domain operator()(const Domain&) const { return val; }
	virtual Domain operator()() const { return val; }
	template<typename T>
	operator T() const { return static_cast<T>(val); }
	template<typename T>
	decltype(auto) Make(const T& t) { return Constant<T>(t);}
private:
	Domain val;
	friend std::ostream& operator<<(std::ostream& s, const Constant& c) { return s<<c.val;  }
};

template<template<typename,typename> class D,typename L, typename R>
class OperationBase 
{
	using LeftType = L;
	using RightType = R;
	using Derived = D<L,R>;
	friend class D<L,R>;
public:
	OperationBase(const RightType& r, const LeftType& l): right{r}, left{l} {}
	//decltype(auto) operator()(const auto& v) { return T::Calculate(v,val); }
private:
	RightType right;
	LeftType left;
};

template<typename L, typename R>
class Add: public OperationBase<Add,L,R>
{
	using Base = OperationBase<Add,L,R>;
	friend class OperationBase<Add,L,R>;
public:
	Add(const L& l, const R& r): Base{l,r} {}
	decltype(auto) operator()(const auto& v) { return Base::left() + Base::right(); }
private:
	//static decltype(auto) calculate(const auto& v, const auto& val) { return v + val; }
};

//template<typename L, typename R>
//class Sub: public OperationBase<Subtraction,Sub,L,R>
//{
//	using Base = OperationBase<Subtraction,Sub,L,R>;
//	friend class OperationBase<Subtraction,Sub,L,R>;
//public:
//	Sub(const L& v): Base{v} {}
//	using Type = decltype(Subtraction::Calculate(std::declval<L>(), std::declval<R>()));
//private:
//	static decltype(auto) calculate(const auto& v, const auto& val) { return v - val; }
//};
//
//template<typename L, typename R>
//class Mul: public OperationBase<Multiplication,Mul,L,R>
//{
//	using Base = OperationBase<Multiplication,Mul,L,R>;
//	friend class OperationBase<Multiplication,Mul,L,R>;
//public:
//	Mul(const L& v): Base{v} {}
//	using Type = decltype(Multiplication::Calculate(std::declval<L>(), std::declval<R>()));
//private:
//	static decltype(auto) calculate(const auto& v, const auto& val) { return v * val; }
//};
//
//template<typename L, typename R>
//class Div: public OperationBase<Division,Div,L,R>
//{
//	using Base = OperationBase<Division,Div,L,R>;
//	friend class OperationBase<Division,Div,L,R>;
//public:
//	Div(const L& v): Base{v} {}
//	using Type = decltype(Division::Calculate(std::declval<L>(), std::declval<R>()));
//private:
//	static decltype(auto) calculate(const auto& v, const auto& val) { return v / val; }
//};
//
//template<typename Op, typename M1, typename V>
//class ValueOperation 
//{
//public:
//	using Left = M1;
//	using Right = V;
//	using ValueType = Op::Type;
//	using DataType = std::shared_ptr<ValueType>;    
//	using DescriptorType = MatrixDescriptor<Left::Order,ValueType>;
//	using MatrixType = Matrix<Left::Order,DescriptorType>;
//	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
//    inline static constexpr Literal LiteralType{TypeIdentifier};
//	static int constexpr Size = Left::Order;
//protected:
//private:
//	template<typename U> using IsT =  Is<U,LiteralType>;
//	friend std::ostream& operator<<(std::ostream& s, const ValueOperation& me) { return s;  }
//};
//template<template<typename,typename> class Op, typename M1, typename M2>
//class MatrixOperation 
//{
//public:
//	using Left = M1;
//	using Right = std::remove_reference<M2>::type;
//	using ValueType = Op<typename Left::ElementType, typename Right::ElementType>::Type;
//	using DataType = std::shared_ptr<ValueType>;    
//	using DescriptorType = MatrixDescriptor<Left::Order,ValueType>;
//	using MatrixType = Matrix<Left::Order,DescriptorType>;
//	inline static constexpr const char TypeIdentifier[] = "MatrixOperation";
//    inline static constexpr Literal LiteralType{TypeIdentifier};
//	static int constexpr Size = Left::Order;
//protected:
//private:
//	template<typename U> using IsT =  Is<U,LiteralType>;
//	friend std::ostream& operator<<(std::ostream& s, const MatrixOperation& me) { return s;  }
//};

