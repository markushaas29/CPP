#include <memory>  
#include "UnaryExpression.hpp"  
#include "BinaryExpression.hpp"  

#pragma once

class AddExp: public BinaryExpression<AddExp,int>
{
    using Base = BinaryExpression<AddExp,int>;
    friend class BinaryExpression<AddExp,int>;
    static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) + r(v); }
public:
    AddExp(typename Base::LeftType l, typename Base::LeftType r): Base{std::move(l),std::move(r)} {}
    virtual int operator()() const { return (*Base::left)() + (*Base::right)(); }
private:
    inline static constexpr const char* sign = "+";
};

class SubExp: public BinaryExpression<SubExp,int>
{
    using Base = BinaryExpression<SubExp,int>;
    friend class BinaryExpression<SubExp,int>;
    static decltype(auto) op(const auto l, const auto r, const auto v) { return l(v) - r(v); }
public:
    SubExp(typename Base::LeftType l, typename Base::LeftType r): Base{std::move(l),std::move(r)} {}
    virtual int operator()() const { return (*Base::left)() - (*Base::right)(); }
private:
    inline static constexpr const char* sign = "-";
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
