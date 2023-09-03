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
	using Type = Functional<D>;
public:
	virtual ~Functional(){};
	decltype(auto) operator()() const
	{ 
		auto cderived = const_cast<Type&>(*this);
		return  static_cast<Derived&>(cderived);
	};
	decltype(auto) operator()(const auto& v) const { return v; }
private:
	friend D;
};

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

template<template<typename,typename> class Op, typename L, typename R>
decltype(auto) Func(const L& l, const R& r) { return Op<L,R>(l,r); }

template<template<typename> class C, typename T>
decltype(auto) Func(const T& t) { return C<T>(t); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator+(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator+(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator+(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator-(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator-(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator-(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator*(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator*(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator*(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator/(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Div>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator/(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Div>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator/(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Div>(f1,f2); }
