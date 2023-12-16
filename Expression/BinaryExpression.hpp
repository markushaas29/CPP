#include "ExpressionBase.hpp" 

#pragma once

template<template<typename,typename> class D,typename L, typename R>
class BinaryExpression: public Expression<BinaryExpression<D,L,R>>
{
	using Derived = D<L,R>;
	using Type = BinaryExpression<D,L,R>;
	using Base = Expression<Type>;
	friend class Expression<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend class D<L,R>;
public:
	using LeftType = L;
	using RightType = R;
	BinaryExpression(const LeftType& l,const RightType& r ): right{r}, left{l} { }
	BinaryExpression(const BinaryExpression& b ): right{b.right}, left{b.left} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	bool operator()() const { return Derived::op(left,right); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
	constexpr decltype(auto) Left() const { return left; }
	constexpr decltype(auto) Right() const { return right; };
private:
	friend std::ostream& operator<<(std::ostream& s, const BinaryExpression& c) { return s<<"{"<<c.left<<" "<<c.sign<<" "<<c.right<<"}";  }
	RightType right;
	LeftType left;
};

template<typename L, typename R>
class And: public BinaryExpression<And,L,R>
{
	using Base = BinaryExpression<And,L,R>;
	friend class BinaryExpression<And,L,R>;
	static decltype(auto) op(const auto l, const auto r) { return l() && r(); }
	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) + r(v); }
public:
	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
	And(const L& l, const R& r): Base{l,r} {}
private:
	inline static constexpr const char* sign = "+"; 
};
//
//template<typename L, typename R>
//class Sub: public BinaryExpression<Sub,L,R>
//{
//	using Base = BinaryExpression<Sub,L,R>;
//	friend class BinaryExpression<Sub,L,R>;
//	inline static constexpr const char* sign = "-"; 
//	static decltype(auto) op(const auto l, const auto r) { return l() - r(); }
//	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) - r(v); }
//public:
//	Sub(const L& l, const R& r): Base{l,r} {}
//	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
//};
//
//template<typename L, typename R>
//class Mul: public BinaryExpression<Mul,L,R>
//{
//	using Base = BinaryExpression<Mul,L,R>;
//	friend class BinaryExpression<Mul,L,R>;
//	inline static constexpr const char* sign = "*"; 
//	static decltype(auto) op(const auto l, const auto r) { return l() * r(); }
//	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) * r(v); }
//public:
//	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
//	Mul(const L& l, const R& r): Base{l,r} {}
//};
//
//template<typename L, typename R>
//class Div: public BinaryExpression<Div,L,R>
//{
//	using Base = BinaryExpression<Div,L,R>;
//	friend class BinaryExpression<Div,L,R>;
//	inline static constexpr const char* sign = "/"; 
//	static decltype(auto) op(const auto l, const auto r) { return l() / r(); }
//	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) / r(v); }
//public:
//	Div(const L& l, const R& r): Base{l,r} {}
//	using ResultType = decltype(op(std::declval<L>(),std::declval<R>()));
//};
//
//constexpr int GCD(const auto x, const auto y) noexcept { return y == 0 ? x : GCD(y,x % y);  }
//
//template<typename L, typename R>
//class Fraction: public BinaryExpression<Fraction,Constant<L>,Constant<R>>
//{
//	using Base = BinaryExpression<Fraction,Constant<L>,Constant<R>>;
//	friend class BinaryExpression<Fraction,Constant<L>,Constant<R>>;
//	inline static constexpr const char* sign = "/"; 
//public:
//	static decltype(auto) op(const auto l, const auto r) { return (double)l() / r(); }
//	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) / r(v); }
//	Fraction(const L& l, const R& r): Base{Constant<L>(l),Constant<R>(r)} {}
//	using ResultType = decltype(op(std::declval<Constant<L>>(),std::declval<Constant<R>>()));
//	constexpr decltype(auto) Num() const { return Base::Left(); }
//	constexpr decltype(auto) Den() const { return Base::Right(); };
//	constexpr decltype(auto) Gcd() const { return GCD(Base::Right()(),Base::Left()()); };
//private:
//	friend std::ostream& operator<<(std::ostream& s, const Fraction& c) { return s<<c.Left()<<sign<<c.Right();  }
//};
