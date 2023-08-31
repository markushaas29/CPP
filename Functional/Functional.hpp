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
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
	OperationBase(const LeftType& l,const RightType& r ): right{r}, left{l} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right); }
	decltype(auto) operator()() const { return Derived::op(left,right); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const OperationBase& c) { return s<<"{"<<c.left<<" "<<c.sign<<" "<<c.right<<"}";  }
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
private:
	inline static constexpr const char* sign = "+"; 
	static decltype(auto) op(const auto l, const auto r) { return l() + r(); }
};

template<typename L, typename R>
class Mul: public OperationBase<Mul,L,R>
{
	using Base = OperationBase<Mul,L,R>;
	friend class OperationBase<Mul,L,R>;
public:
	Mul(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "*"; 
	static decltype(auto) op(const auto l, const auto r) { return l() * r(); }
};

