#include <memory>  
#include "UnaryExpression.hpp"  
#include "BinaryExpression.hpp"  

#pragma once

template<class> class Fx;

template<typename T>
class Var: public IExpression<T>
{
	using Type = Var<T>;
public:
	Var(bool v): value{v} {}
	virtual T operator()() const { return value; }
	virtual std::shared_ptr<IExpression<T>> Clone()  { return std::make_shared<Type>(value); }
	decltype(auto) operator()(bool v) { value = v; }
	template<typename TO>
	explicit operator TO() const { return static_cast<TO>((*this)()); }
	decltype(auto) operator==(const Type& u){ return (*this)() == u();	}
protected:
    std::ostream& display(std::ostream& s) const { return s<<value;  } 
private:
	friend std::ostream& operator<<(std::ostream& s, const Type& c) { return s<<c.value;  }
	T value;
};

class Not: public UnaryExpression<Not,bool>
{
	using Type = Not;
	using Base = UnaryExpression<Not,bool>;
	friend class UnaryExpression<Not,bool>;
public:
	Not(std::shared_ptr<typename Base::Interface> v): Base{std::move(v)} {}
	virtual bool operator()() const { return (*Base::value)(); }
private:
};

class And: public BinaryExpression<And,bool>
{
    using Base = BinaryExpression<And,bool>;
    friend class BinaryExpression<And,bool>;
    static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) + r(v); }
public:
    And(typename Base::LeftType l, typename Base::LeftType r): Base{std::move(l),std::move(r)} {}
    virtual bool operator()() const { return (*Base::left)() && (*Base::right)(); }
private:
    inline static constexpr const char* sign = "&&";
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
