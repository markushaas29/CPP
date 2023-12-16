#pragma once

class IExpression
{
public:
	virtual ~IExpression() = 0;
	virtual bool operator()() const = 0;
	virtual bool operator()(bool v) const = 0; 
};

template<typename D>
class Expression: IExpression
{
	using Derived = D;
	using Type = Expression<D>;
public:
	virtual ~Expression(){};
	virtual bool operator()() const
	{ 
		auto cderived = const_cast<Type&>(*this);
		return  static_cast<Derived&>(cderived);
	};
	virtual bool operator()(bool v) const { return v; }
private:
	friend D;
};
