#include <memory>  
#include "ExpressionBase.hpp"  
#include "../Matrix/PointerConcept.hpp"  

#pragma once

template<class> class Fx;

template<template<typename> class D,typename V>
class UnaryExpression: public Expression<UnaryExpression<D,V>>
{
	using ValueType = V;
	using Derived = D<V>;
	using Type = UnaryExpression<D,V>;
	using Base = Expression<Type>;
	friend class Expression<Type>;
	template<typename> friend class D;
	inline static constexpr const char* sign = Derived::sign; 
public:
	UnaryExpression(const ValueType& v): value{v} {}
	decltype(auto) operator()(const auto& v) const 
	{
		if constexpr (std::is_same_v<D<V>,Fx<V>>)
			return Derived::op(v); 
		else
			return Derived::op(value); 
	}
	decltype(auto) operator()() const 
	{ 
		if constexpr(std::is_invocable_v<ValueType>)
			return value();
		else
			return Derived::op(value); 
	}
	template<typename T>
	explicit operator T() const { return static_cast<T>((*this)()); }
	template<template<typename> class D1,typename V1>
	decltype(auto) operator<=>(const UnaryExpression<D1,V1>& u){ return (*this)() <=> u();	}
	template<template<typename> class D1,typename V1>
	decltype(auto) operator==(const UnaryExpression<D1,V1>& u){ return (*this)() == u();	}
private:
	friend std::ostream& operator<<(std::ostream& s, const UnaryExpression& c) { return s<<c.value;  }
	ValueType value;
};

template<class Domain=double>
class Constant: public UnaryExpression<Constant,Domain>
{
	using Type = Constant<Domain>;
	using Base = UnaryExpression<Constant, Domain>;
	friend class UnaryExpression<Constant, Domain>;
public:
	Constant(const Domain& v): Base{v} {}
	template<typename T>
	Constant(const Constant<T>& c): Base{c()} {}
	template<typename T>
	Constant(std::is_arithmetic<T> v): Base{static_cast<Domain>(v)} {} 
private:
	static decltype(auto) op(const auto& v) { return v; }
};

template<class Domain=double>
class Parameter: public UnaryExpression<Parameter, std::shared_ptr<Domain>>
{
	using Type = Parameter<Domain>;
	using Base = UnaryExpression<Parameter, std::shared_ptr<Domain>>;
	template<template<typename> class, typename> friend class UnaryExpression;
public:
	Parameter(const Domain& v): Base{std::make_shared<Domain>(v)} {}
	Parameter(std::shared_ptr<Domain> v): Base{v} {} 
	Parameter(const Parameter& p): Base(p.value) { } 
	Domain& Value() const { return *Base::value; }
	template<typename T>
	explicit operator T() const { return static_cast<T>(*Base::value); }
private:
	friend std::ostream& operator<<(std::ostream& s, const Parameter& c) { return s<<c.Value();  }
	static decltype(auto) op(const auto& v) { return *v; }
};

template<class Domain=double>
class Fx: public UnaryExpression<Fx, Domain>
{
	using Type = Fx<Domain>;
	using Base = UnaryExpression<Fx, Domain>;
	template<template<typename> class, typename> friend class UnaryExpression;
public:
	Fx(const Domain& v = Domain{}): Base{v} {}
private:
	static decltype(auto) op(const auto& v) { return v; }
};
