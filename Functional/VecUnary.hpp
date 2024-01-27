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
	decltype(auto) operator()(const auto& v) const { return cast().op(value,v); }
	decltype(auto) operator()() const 	{	return cast().op(value); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VecUnary& c) 
	{
		s<<"{";
		for(size_t i= 0;i < c.value.size(); ++i)
			s<<c.value[i]<<(i < (c.value.size()-1) ? Derived::sign : "");
		return s<<"}";  
	}
	Derived cast() const                                                                                                 
    { 
         auto cderived = const_cast<Type&>(*this);                                                                                                       
         return  static_cast<Derived&>(cderived);   
    }
	VecType value;
};

template<typename V>
class Acc: public VecUnary<Acc,V>
{
	using Base = VecUnary<Acc,V>;
	friend class VecUnary<Acc,V>;
public:
	inline static constexpr const char* sign = "+";
	Acc(const Base::VecType& v): Base{v} {}
private:
	template<typename T>
	decltype(auto) op(const std::vector<T>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(i); });
		return r; 
	}
	
	template<typename T>
	decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(*i); });
		return Acc<V>(v1); 
	}
};

template<typename V>
class Diff: public VecUnary<Diff,V>
{
	using Base = VecUnary<Diff,V>;
	friend class VecUnary<Diff,V>;
public:
	inline static constexpr const char* sign = "-";
	using ResultType = Sub<Constant<V>,Constant<V>>;
	Diff(const Base::VecType& v): Base{v} {}

private:
	template<typename T, typename U=T>
	decltype(auto) op(const std::vector<T>& v1) 
	{ 
		std::vector<ResultType> result;
		for(uint i =0; i < (v1.size() - 1); ++i)
			result.push_back(ResultType(Constant(v1[i]),Constant(v1[i+1])));
		return result; 
	}
	
	template<typename T, typename U=T>
	decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1) 
	{ 
		std::vector<ResultType> result;
		for(uint i =0; i < (v1.size() - 1); ++i)
			result.push_back(ResultType(Constant(*v1[i]),Constant(*v1[i+1])));
		return result; 
	}
};

