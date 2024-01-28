#include <memory>
#include <iostream>

#pragma once

class ICounter
{
public:
	virtual ~ICounter() {};
private:
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<template<typename,typename> class D,typename L, typename R>
class CounterBase: public ICounter
{
	using Derived = D<L,R>;
	using Type = CounterBase<D,L,R>;
public:
	CounterBase() {}
private:
	friend std::ostream& operator<<(std::ostream& s, const CounterBase<D,L,R>& c) { return s<<c.cast().create(c.left,c.right);	}
	std::ostream& display(std::ostream& s) const { return s; };
	Derived cast() const 
	{ 
		 auto cderived = const_cast<Type&>(*this);
         return  static_cast<Derived&>(cderived);
	}
};


