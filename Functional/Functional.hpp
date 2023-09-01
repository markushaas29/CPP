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

template<typename D>
class Functional
{
	using Derived = D;
public:
	virtual ~Functional(){};
private:
	decltype(auto) operator()() const { return ; }
	decltype(auto) operator()(const auto& v) const { return v; }
	friend D;
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
class BinaryFunctional: public Functional<BinaryFunctional<D,L,R>>
{
	using LeftType = L;
	using RightType = R;
	using Derived = D<L,R>;
	using Type = BinaryFunctional<D,L,R>;
	using Base = Functional<Type>;
	friend class Functional<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
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
public:
	Add(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "+"; 
	static decltype(auto) op(const auto l, const auto r) { return l() + r(); }
};

template<typename L, typename R>
class Sub: public BinaryFunctional<Sub,L,R>
{
	using Base = BinaryFunctional<Sub,L,R>;
	friend class BinaryFunctional<Sub,L,R>;
public:
	Sub(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "-"; 
	static decltype(auto) op(const auto l, const auto r) { return l() - r(); }
};

template<typename L, typename R>
class Mul: public BinaryFunctional<Mul,L,R>
{
	using Base = BinaryFunctional<Mul,L,R>;
	friend class BinaryFunctional<Mul,L,R>;
public:
	Mul(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "*"; 
	static decltype(auto) op(const auto l, const auto r) { return l() * r(); }
};

template<typename L, typename R>
class Div: public BinaryFunctional<Div,L,R>
{
	using Base = BinaryFunctional<Div,L,R>;
	friend class BinaryFunctional<Div,L,R>;
public:
	Div(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "/"; 
	static decltype(auto) op(const auto l, const auto r) { return l() / r(); }
};

template<template<typename,typename> class Op, typename L, typename R>
decltype(auto) Func(const L& l, const R& r) { return Op<L,R>(l,r); }

template<template<typename> class C, typename T>
decltype(auto) Func(const T& t) { return C<T>(t); }
