#include "FunctionalBase.hpp" 

#pragma once

template<template<typename,typename> class D,typename L, typename R>
class BinaryFunctional: public Functional<BinaryFunctional<D,L,R>>
{
	using Derived = D<L,R>;
	using Type = BinaryFunctional<D,L,R>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
	using LeftType = L;
	using RightType = R;
	BinaryFunctional(const LeftType& l,const RightType& r ): right{r}, left{l} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right); }
	decltype(auto) operator()() const { return Derived::op(left,right); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const BinaryFunctional& c) { return s<<"{"<<c.left<<" "<<c.sign<<" "<<c.right<<"}";  }
	RightType right;
	LeftType left;
};

template<typename L, typename R>
class Add: public BinaryFunctional<Add,L,R>
{
	using Base = BinaryFunctional<Add,L,R>;
	friend class BinaryFunctional<Add,L,R>;
	static decltype(auto) op(const auto l, const auto r) { return l() + r(); }
public:
	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
	Add(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "+"; 
};

template<typename L, typename R>
class Sub: public BinaryFunctional<Sub,L,R>
{
	using Base = BinaryFunctional<Sub,L,R>;
	friend class BinaryFunctional<Sub,L,R>;
	inline static constexpr const char* sign = "-"; 
	static decltype(auto) op(const auto l, const auto r) { return l() - r(); }
public:
	Sub(const L& l, const R& r): Base{l,r} {}
	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
};

template<typename L, typename R>
class Mul: public BinaryFunctional<Mul,L,R>
{
	using Base = BinaryFunctional<Mul,L,R>;
	friend class BinaryFunctional<Mul,L,R>;
	inline static constexpr const char* sign = "*"; 
	static decltype(auto) op(const auto l, const auto r) { return l() * r(); }
public:
	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
	Mul(const L& l, const R& r): Base{l,r} {}
};

template<typename L, typename R>
class Div: public BinaryFunctional<Div,L,R>
{
	using Base = BinaryFunctional<Div,L,R>;
	friend class BinaryFunctional<Div,L,R>;
	inline static constexpr const char* sign = "/"; 
	static decltype(auto) op(const auto l, const auto r) { return l() / r(); }
public:
	Div(const L& l, const R& r): Base{l,r} {}
	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
};