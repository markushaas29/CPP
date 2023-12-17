#include <memory>  
#include "ExpressionBase.hpp"  
#include "../Matrix/PointerConcept.hpp"  

#pragma once

template<class> class Fx;

class Var: public IExpression
{
	using Type = Var;
public:
	Var(bool v): value{v} {}
	virtual bool operator()() const { return value; }
	virtual std::shared_ptr<IExpression> Clone()  { return std::make_shared<Var>(value); }
	decltype(auto) operator()(bool v) { value = v; }
	template<typename T>
	explicit operator T() const { return static_cast<T>((*this)()); }
	decltype(auto) operator==(const Var& u){ return (*this)() == u();	}
private:
	friend std::ostream& operator<<(std::ostream& s, const Var& c) { return s<<c.value;  }
	bool value;
};

template<typename D>
class UnaryExpression: public IExpression
{
	using Derived = D;
	using Type = UnaryExpression<D>;
	friend Derived;
	inline static constexpr const char* sign = Derived::sign; 
public:
	UnaryExpression(std::shared_ptr<IExpression> v): value{std::move(v)} {}
	template<typename T>
	explicit operator T() const { return static_cast<T>((*this)()); }
	template<typename D1>
	decltype(auto) operator==(const UnaryExpression<D1>& u){ return (*this)() == u();	}
	virtual std::shared_ptr<IExpression> Clone()  { return std::make_shared<Derived>(value); }
private:
	friend std::ostream& operator<<(std::ostream& s, const UnaryExpression& c) { return s<<c.value;  }
	std::shared_ptr<IExpression> value;
};

class Not: public UnaryExpression<Not>
{
	using Type = Not;
	using Base = UnaryExpression<Not>;
	friend class UnaryExpression<Not>;
public:
	Not(std::shared_ptr<IExpression> v): Base{std::move(v)} {}
	virtual bool operator()() const { return (*Base::value)(); }
private:
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
