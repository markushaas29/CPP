#include "FunctionalBase.hpp" 
#include "Binary.hpp" 

#pragma once

template<template<typename,typename> class D,typename L, typename R>
class VectorFunctional: public Functional<VectorFunctional<D,L,R>>
{
	using Derived = D<L,R>;
	using Type = VectorFunctional<D,L,R>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
	using LeftType = L;
	using RightType = R;
	VectorFunctional(const LeftType& l,const RightType& r ): right{r}, left{l} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	decltype(auto) operator()() const { return Derived::op(left,right); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VectorFunctional& c) { return s<<"{"<<c.left<<" "<<c.sign<<" "<<c.right<<"}";  }
	RightType right;
	LeftType left;
};


template<typename L, typename R>
class Acc: public VectorFunctional<Acc,L,R>
{
	using Base = BinaryFunctional<Acc,L,R>;
	friend class BinaryFunctional<Acc,L,R>;
public:
	using Type = Acc;
	using LeftType = L;
	using RightType = R;

	Acc(const L& l, const R& r): Base{l,r} {}

	template<typename T>
	static constexpr decltype(auto) op(const std::vector<T>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(i); });
		return Acc(v1,r); 
	}
	
	template<typename T>
	static constexpr decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1) 
	{ 
		double r = 0;
		std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(*i); });
		return Acc<L,R>(v1,r); 
	}
private:
};
template<typename L, typename R>
class Dot
{
public:
	using Type = Dot<L,R>;
	using LeftType = L;
	using RightType = R;

	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<T>& v1, const std::vector<U>& v2) 
	{ 
		using RT = Mul<T,U>;
		std::vector<RT> inter;
		for(uint i =0; i < v1.size(); ++i)
			inter.push_back(Mul<T,U>(v1[i],v2[i]));

		return Acc(inter); 
	}
	
	template<typename T, typename U=T>
	static constexpr decltype(auto) op(const std::vector<std::shared_ptr<T>>& v1, const std::vector<std::shared_ptr<U>>& v2) 
	{ 
		using RT = Mul<T,U>;
		std::vector<std::shared_ptr<RT>> inter;
		for(uint i =0; i < v1.size(); ++i)
			inter.push_back(std::make_shared<RT>(Mul<T,U>(*v1[i],*v2[i])));

		return (double)Acc<T,U>::op(inter); 
	}

//	template<typename T>
//	constexpr operator T() const { return static_cast<T>(value); }
//	std::ostream& Display(std::ostream& strm) const	
//	{
//		for(uint i = 0; i < left.size(); ++i)
//			strm<<"{"<<*left[i]<<"}"<<( (i+1) != left.size() ? "+" : "");
//		strm<<" = "<<value;
//		return strm; 
//	}
private:
};


