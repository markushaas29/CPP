#pragma once

template<typename D>
class Expression
{
	using Derived = D;
	using Type = Expression<D>;
public:
	virtual ~Expression(){};
	decltype(auto) operator()() const
	{ 
		auto cderived = const_cast<Type&>(*this);
		return  static_cast<Derived&>(cderived);
	};
	decltype(auto) operator()(const auto& v) const { return v; }
private:
	friend D;
};
