#include <memory>  
#include "ExpressionBase.hpp"  
#include "../Matrix/PointerConcept.hpp"  

#pragma once

template<class> class Fx;

template<typename D,typename V>
class UnaryExpression: public Expression<UnaryExpression<D,V>>
{
	using ValueType = V;
	using Derived = D;
	using Type = UnaryExpression<D,V>;
	using Base = Expression<Type>;
	friend class Expression<Type>;
	friend Derived;
	inline static constexpr const char* sign = Derived::sign; 
public:
	UnaryExpression(const ValueType& v): value{v} {}
	decltype(auto) operator()(const auto& v) const 
	{
//		if constexpr (std::is_same_v<D>,Fx<V>>)
//			return Derived::op(v); 
//		else
//			return Derived::op(value); 
	}
	bool operator()() const 
	{ 
		if constexpr(std::is_invocable_v<ValueType>)
			return value();
		else
			return Derived::op(value); 
	}
	template<typename T>
	explicit operator T() const { return static_cast<T>((*this)()); }
	template<typename D1,typename V1>
	decltype(auto) operator==(const UnaryExpression<D1,V1>& u){ return (*this)() == u();	}
private:
	friend std::ostream& operator<<(std::ostream& s, const UnaryExpression& c) { return s<<c.value;  }
	ValueType value;
};

class BooleanExpression: public UnaryExpression<BooleanExpression,bool>
{
	using Type = BooleanExpression;
	using Base = UnaryExpression<BooleanExpression, bool>;
	friend class UnaryExpression<BooleanExpression, bool>;
public:
	BooleanExpression(bool v): Base{v} {}
private:
	static decltype(auto) op(bool v) { return v; }
};
//
//template<class Domain=double>
//class Parameter: public UnaryExpression<Parameter, std::shared_ptr<Domain>>
//{
//	using Type = Parameter<Domain>;
//	using Base = UnaryExpression<Parameter, std::shared_ptr<Domain>>;
//	template<template<typename> class, typename> friend class UnaryExpression;
//public:
//	Parameter(const Domain& v): Base{std::make_shared<Domain>(v)} {}
//	Parameter(std::shared_ptr<Domain> v): Base{v} {} 
//	Parameter(const Parameter& p): Base(p.value) { } 
//	Domain& Value() const { return *Base::value; }
//	template<typename T>
//	explicit operator T() const { return static_cast<T>(*Base::value); }
//private:
//	friend std::ostream& operator<<(std::ostream& s, const Parameter& c) { return s<<c.Value();  }
//	static decltype(auto) op(const auto& v) { return *v; }
//};
//
//template<class Domain=double>
//class Fx: public UnaryExpression<Fx, Domain>
//{
//	using Type = Fx<Domain>;
//	using Base = UnaryExpression<Fx, Domain>;
//	template<template<typename> class, typename> friend class UnaryExpression;
//public:
//	Fx(const Domain& v = Domain{}): Base{v} {}
//private:
//	static decltype(auto) op(const auto& v) { return v; }
//};
