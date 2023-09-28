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
	friend std::ostream& operator<<(std::ostream& s, const VecUnary& c) { return s<<"{"<<"}";  }
	VecType value;
};
template<uint N, template<typename,typename> class D,typename L, typename R>
class VectorFunctional: public Functional<VectorFunctional<N,D,L,R>>
{
	using Derived = D<L,R>;
	using Type = VectorFunctional<N,D,L,R>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	inline static constexpr uint VecNum = N; 
	friend class D<L,R>;
public:
	using LeftType = L;
	using LVecType = std::vector<L>;
	using RightType = R;
	using RVecType = std::vector<R>;
	VectorFunctional(const LVecType& l,const RVecType& r ): right{r}, left{l} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	decltype(auto) operator()() const 
	{ 
		if constexpr (N==1)
			return Derived::op(left);
		else
			return Derived::op(left,right); 
	}
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VectorFunctional& c) { return s<<"{"<<c.left<<" "<<c.sign<<" "<<c.right<<"}";  }
	RVecType right;
	LVecType left;
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
template<typename L, typename R>
class Dot: public VectorFunctional<2,Dot,L,R>
{
	using Base = VectorFunctional<2,Dot,L,R>;
	friend class VectorFunctional<2,Dot,L,R>;
public:
	using Type = Dot<L,R>;
	using LeftType = L;
	using RightType = R;
	//using LType = typename L::value_type;
	//using RType = typename R::value_type;
	//using ResultType = Mul<Constant<LType>,Constant<RType>>;
	
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


