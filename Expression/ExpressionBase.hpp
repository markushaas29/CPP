#pragma once

class IExpression
{
public:
	//virtual ~IExpression() = 0;
	virtual bool operator()() const = 0;
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
