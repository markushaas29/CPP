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
