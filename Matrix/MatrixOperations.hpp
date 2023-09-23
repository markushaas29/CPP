#include <memory>
#include <tuple>
#include <vector>
#include <type_traits>
#include "MatrixElement.hpp"
#include "../Functional/Binary.hpp"
#include "../Functional/Unary.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#pragma once

template<size_t, typename> class MatrixDescriptor;
template<size_t, typename> class Matrix;

template<template<typename,typename> class T, template<typename,typename> class D,typename L, typename R>
class OperationBase 
{
	using LeftType = L;
	using RightType = R;
	using Type = T<Constant<L>,Constant<R>>;
	using OpType = D<L,R>;
public:
	OperationBase(const RightType v): val{v} {	}
	decltype(auto) operator()(const auto& v) const { return Type(v,val); }
	decltype(auto) operator()(const auto& v, const auto& v2) const { std::cout<<v<<" "<<v2<<" "<<Type(v,v2)<<std::endl;return Type(v,v2); }
private:
	friend std::ostream& operator<<(std::ostream& s, const OperationBase& o) { return s<<o.val;  }
	RightType val;
};

template<typename L, typename R>
class ElementAdd: public OperationBase<Add,ElementAdd,L,R>
{
	using Base = OperationBase<Add,ElementAdd,L,R>;
	friend class OperationBase<Add,ElementAdd,L,R>;
public:
	ElementAdd(const L v): Base{v} {}
	using Type = Add<Constant<L>, Constant<R>>;
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v + val; }
};

template<typename L, typename R>
class ElementSub: public OperationBase<Sub,ElementSub,L,R>
{
	using Base = OperationBase<Sub,ElementSub,L,R>;
	friend class OperationBase<Sub,ElementSub,L,R>;
public:
	ElementSub(const L& v): Base{v} {}
	using Type = Sub<Constant<L>, Constant<R>>;
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v - val; }
};

template<typename L, typename R>
class ElementMul: public OperationBase<Mul,ElementMul,L,R>
{
	using Base = OperationBase<Mul,ElementMul,L,R>;
	friend class OperationBase<Mul,ElementMul,L,R>;
public:
	ElementMul(const L& v): Base{v} {}
	using Type = Mul<Constant<L>, Constant<R>>;
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v * val; }
};

template<typename L, typename R>
class ElementDiv: public OperationBase<Div,ElementDiv,L,R>
{
	using Base = OperationBase<Div,ElementDiv,L,R>;
	friend class OperationBase<Div,ElementDiv,L,R>;
public:
	ElementDiv(const L& v): Base{v} {}
	using Type = Div<Constant<L>, Constant<R>>;
private:
	static decltype(auto) calculate(const auto& v, const auto& val) { return v / val; }
};

template<template<typename,typename> class D,typename L, typename R>
class MatrixOpBase 
{
	using LeftType = L;
	using RightType = R;
	using OpType = D<L,R>;
public:
	MatrixOpBase(const RightType& v): val{v} {}
	decltype(auto) operator()(Constant<L> l,Constant<R> r) { return OpType::calculate(Constant<L>(l),Constant<R>(r)); }
private:
	RightType val;
};

template<typename L, typename R=L>
class Diff: public MatrixOpBase<Diff,L,R>
{
	using Base = MatrixOpBase<Diff,L,R>;
	friend class MatrixOpBase<Diff,L,R>;
	using Op = Sub<Constant<L>,Constant<R>>;
public:
	using ResultType = Op;//::ResultType;
	Diff(const L& v = L{}): Base{v} {}
	using Type = Div<L,R>;
private:
	static decltype(auto) calculate(const auto& l, const auto& r) { return Op(l,r)(); }
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

