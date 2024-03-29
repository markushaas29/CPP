#include "FunctionalBase.hpp" 
#include "Unary.hpp" 
#include "Binary.hpp" 

#pragma once

template<template<typename> class D,typename V>
class VecUnary: public Functional<VecUnary<D,V>>
{
	using Derived = D<V>;
	using Type = VecUnary<D,V>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<V>;
public:
	using ValueType = V;
	using VecType = std::vector<V>;
	VecUnary(const VecType& v): value{v} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(value,v); }
	decltype(auto) operator()() const 	{	return Derived::op(value); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VecUnary& c) 
	{ 
		auto v = c();
		std::for_each(v.cbegin(), v.cend(), [&](const auto& i) { s<<i; });
		return s;  
	}
	VecType value;
};

template<typename V>
class Acc: public VecUnary<Acc,V>
{
	using Base = VecUnary<Acc,V>;
	friend class VecUnary<Acc,V>;
public:
	Acc(const Base::VecType& v): Base{v} {}

	template<typename T>
	static constexpr decltype(auto) op(const std::vector<T>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(i); });
		return r; 
	}
	
	template<typename T>
	static constexpr decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(*i); });
		return Acc<V>(v1); 
	}
private:
};

template<typename V>
class Diff: public VecUnary<Diff,V>
{
	using Base = VecUnary<Diff,V>;
	friend class VecUnary<Diff,V>;
public:
	using ResultType = Sub<Constant<V>,Constant<V>>;
	
	Diff(const Base::VecType& v): Base{v} {}

	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<T>& v1) 
	{ 
		std::vector<ResultType> result;
		for(uint i =0; i < (v1.size() - 1); ++i)
			result.push_back(ResultType(Constant(v1[i]),Constant(v1[i+1])));
		return result; 
	}
	
	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1) 
	{ 
		std::vector<ResultType> result;
		for(uint i =0; i < (v1.size() - 1); ++i)
			result.push_back(ResultType(Constant(*v1[i]),Constant(*v1[i+1])));
		return result; 
	}
};

