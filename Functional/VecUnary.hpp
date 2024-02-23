#include "FunctionalBase.hpp" 
#include "Unary.hpp" 
#include "Binary.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 

#pragma once

template<template<typename,typename> class D,typename V, typename VEC = V>
class VecUnary: public Functional<VecUnary<D,V>>
{
	using Derived = D<V,VEC>;
	using Type = VecUnary<D,V,VEC>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<V,VEC>;
	inline static constexpr const char TypeIdentifier[] = "VecUnary";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	using ValueType = V;
	using VecType = std::vector<VEC>;
	decltype(auto) Push(const V& v) { value.push_back(v); }
	decltype(auto) Size() { return value.size(); }
	decltype(auto) Begin() { return value.begin(); }
	decltype(auto) End() { return value.end(); }
	decltype(auto) operator()(const auto& v) const { return cast().op(value,v); }
	decltype(auto) operator()() const 	{	return cast().op(value); }
	decltype(auto) operator()(int i, int j) const 	
	{
		IsT<Throwing>(Format("Index begin: ",i," end ",j, " exceeds size: ", value.size()))(i >= 0 && j <= value.size());
		return cast().op(VecType(value.begin()+i, value.begin()+j)); 
	}
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
protected:
	VecUnary(const VecType& v): value{v} {}
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

template<typename V, typename VEC = V>
class Acc: public VecUnary<Acc,V,VEC>
{
	using Base = VecUnary<Acc,V,VEC>;
	friend class VecUnary<Acc,V,VEC>;
public:
	inline static constexpr const char* sign = "+";
	Acc(const Base::VecType& v =  typename Base::VecType()): Base{v} {}
private:
	template<typename T>
	decltype(auto) op(const std::vector<T>& v1) 
	{ 
		double r = 0;
		if constexpr (std::is_invocable_v<T>)
			std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(i()); });
		else
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

template<typename T>
Acc(const std::vector<T>&) -> Acc<T>;

template<typename T>
Acc(const std::vector<Acc<T>>&) -> Acc<T,Acc<T>>;

template<typename V, typename VEC = V>
class Diff: public VecUnary<Diff,V,V>
{
	using Base = VecUnary<Diff,V>;
	friend class VecUnary<Diff,V>;
public:
	inline static constexpr const char* sign = "-";
	using ResultType = Sub<Constant<V>,Constant<V>>;
	Diff(const Base::VecType& v = typename Base::VecType()): Base{v} {}

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

template<typename T>
Diff(const std::vector<T>&) -> Diff<T>;

