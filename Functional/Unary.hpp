#include "FunctionalBase.hpp"  

#pragma once

template<template<typename> class D,typename V>
class UnaryFunctional: public Functional<UnaryFunctional<D,V>>
{
	using ValueType = V;
	using Derived = D<V>;
	using Type = UnaryFunctional<D,V>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	friend class D<V>;
	inline static constexpr const char* sign = Derived::sign; 
public:
	UnaryFunctional(const ValueType& v): value{v} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(value); }
	decltype(auto) operator()() const { return Derived::op(value); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const UnaryFunctional& c) { return s<<"{"<<c.value<<"}";  }
	ValueType value;
};

template<class Domain=double>
class Constant: public UnaryFunctional<Constant,Domain>
{
	using Type = Constant<Domain>;
	using Base = UnaryFunctional<Constant, Domain>;
	friend class UnaryFunctional<Constant, Domain>;
public:
	Constant(const Domain& v): Base{v} {}
	template<typename T>
	Constant(std::is_arithmetic<T> v): Base{static_cast<Domain>(v)} {} 
private:
	static decltype(auto) op(const auto& v) { return v; }
};
