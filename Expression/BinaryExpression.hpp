#include "ExpressionBase.hpp" 

#pragma once

template<typename D, typename T>
class BinaryExpression: public IExpression<T>
{
	using Derived = D;
	using Type = BinaryExpression<D,T>;
	using Interface = IExpression<T>;
	inline static constexpr const char* sign = Derived::sign; 
	friend D;
public:
	using LeftType = std::shared_ptr<Interface>;
	using RightType = std::shared_ptr<Interface>;
	BinaryExpression(LeftType l, RightType r): right{std::move(r)}, left{std::move(l)} { }
	BinaryExpression(const BinaryExpression& b ): right{b.right}, left{b.left} {}
	decltype(auto) operator()(const auto& v) const { return Derived::op(left,right,v); }
	template<typename TO>
	operator TO() const { return static_cast<TO>((*this)()); }
	virtual std::shared_ptr<Interface> Clone()  { return std::make_shared<Derived>(left,right); } 
	constexpr decltype(auto) Left() const { return left; }
	constexpr decltype(auto) Right() const { return right; };
protected:
	std::ostream& display(std::ostream& s) const { return s<<*(left)<<" "<<sign<<" "<<*(right);  }
private:
	RightType right;
	LeftType left;
};
