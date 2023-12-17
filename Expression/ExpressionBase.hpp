#include <memory>
#pragma once

class IExpression
{
public:
	virtual std::shared_ptr<IExpression> Clone() = 0;
	virtual bool operator()() const = 0;
private:
	friend bool operator==(std::shared_ptr<IExpression> e1, std::shared_ptr<IExpression> e2) { std::cout<<"COM"<<std::endl; return (*e1)()==(*e2)(); };
};

template<typename D>
class Expression: public IExpression
{
	using Derived = D;
	using Type = Expression<D>;
public:
	~Expression(){};
//	decltype(auto) operator()() const
//	{ 
//		auto cderived = const_cast<Type&>(*this);
//		return  static_cast<Derived&>(cderived);
//	};
	decltype(auto) operator()(const auto& v) const { return v; }
private:
	friend D;
};
