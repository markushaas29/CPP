#include "FunctionalBase.hpp" 
#include "Unary.hpp" 
#include "Binary.hpp" 
#include "VecUnary.hpp" 

#pragma once

template<template<typename,typename> class D,typename L, typename R>
class VecBinary: public Functional<VecBinary<D,L,R>>
{
	using Derived = D<L,R>;
	using Type = VecBinary<D,L,R>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
	using LeftType = L;
	using LVecType = std::vector<L>;
	using RightType = R;
	using RVecType = std::vector<R>;
	VecBinary(const LVecType& l,const RVecType& r ): right{r}, left{l} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	decltype(auto) operator()() const 	{ return Derived::op(left,right); 	}
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VecBinary& c) 
	{ 
		auto v = c();
        std::for_each(v.cbegin(), v.cend(), [&](const auto& i) { s<<i; });            
        return s;
	}
	RVecType right;
	LVecType left;
};

template<typename L, typename R>
class Dot: public VecBinary<Dot,L,R>
{
	using Base = VecBinary<Dot,L,R>;
	friend class VecBinary<Dot,L,R>;
public:
	using Type = Dot<L,R>;
	using LeftType = L;
	using RightType = R;
	
	Dot(const Base::LVecType& l, const Base::RVecType& r): Base{l,r} {}

	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<T>& v1, const std::vector<U>& v2) 
	{ 
		using RT = Mul<Constant<T>,Constant<U>>;
        std::vector<RT> inter;
		for(uint i =0; i < v1.size(); ++i)
			inter.push_back(Mul<Constant<T>,Constant<U>>(Constant(v1[i]),Constant(v2[i])));

		return Acc<RT>(inter)(); 
	}
	
	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1, const std::vector<std::shared_ptr<U>>& v2) 
	{ 
		using RT = Mul<Constant<T>,Constant<U>>;
        std::vector<RT> inter;
		for(uint i =0; i < v1.size(); ++i)
			inter.push_back(Mul<Constant<T>,Constant<U>>(*v1[i],*v2[i]));

		return Acc<RT>(inter)(); 
	}

	friend std::ostream& operator<<(std::ostream& s, const Dot& c) { return s<<"{"<<c()<<"}";  }
private:
};


