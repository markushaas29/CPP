#include "ExpressionBase.hpp" 

#pragma once

template<typename D>
class BinaryExpression: public Expression<BinaryExpression<D>>
{
	using Derived = D;
	using Type = BinaryExpression<D>;
	using Base = Expression<Type>;
	friend class Expression<Type>;
	inline static constexpr const char* sign = Derived::sign; 
	friend D;
public:
	using LeftType = std::shared_ptr<IExpression>;
	using RightType = std::shared_ptr<IExpression>;
	BinaryExpression(LeftType l, RightType r): right{std::move(r)}, left{std::move(l)} { }
	BinaryExpression(const BinaryExpression& b ): right{b.right}, left{b.left} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	template<typename T>
	operator T() const { return static_cast<T>((*this)()); }
	constexpr decltype(auto) Left() const { return left; }
	constexpr decltype(auto) Right() const { return right; };
private:
	//friend std::ostream& operator<<(std::ostream& s, const BinaryExpression& c) { return s<<"{"<<*(c.left)<<" "<<c.sign<<" "<<*(c.right)<<"}";  }
	RightType right;
	LeftType left;
};

class And: public BinaryExpression<And>
{
	using Base = BinaryExpression<And>;
	friend class BinaryExpression<And>;
	static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) + r(v); }
public:
	And(typename Base::LeftType l, typename Base::LeftType r): Base{std::move(l),std::move(r)} {}
	virtual bool operator()() const { return (*Base::left)() && (*Base::right)(); }
private:
	inline static constexpr const char* sign = "&&"; 
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
