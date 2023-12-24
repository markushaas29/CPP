#include <memory>  
#include "ExpressionBase.hpp"  
#include "../Matrix/PointerConcept.hpp"  

#pragma once

template<typename D, typename T>
class UnaryExpression: public IExpression<T>
{
	using Derived = D;
	using Interface = IExpression<T>;
	using Type = UnaryExpression<D,T>;
	friend Derived;
	inline static constexpr const char* sign = Derived::sign; 
public:
	UnaryExpression(std::shared_ptr<Interface> v): value{std::move(v)} {}
	template<typename TO>
	explicit operator TO() const { return static_cast<TO>((*this)()); }
	template<typename D1>
	decltype(auto) operator==(const Type& u){ return (*this)() == u();	}
	virtual std::shared_ptr<Interface> Clone()  { return std::make_shared<Derived>(value); }
protected:
    std::ostream& display(std::ostream& s) const { return s<<*value;  } 
private:
	friend std::ostream& operator<<(std::ostream& s, const UnaryExpression& c) { return s<<c.value;  }
	std::shared_ptr<Interface> value;
};
  
template<typename T>
class Var: public IExpression<T>
{
    using Type = Var<T>;
public:
    Var(T v): value{v} {}
    virtual T operator()() const { return value; }
    virtual std::shared_ptr<IExpression<T>> Clone()  { return std::make_shared<Type>(value); }
    decltype(auto) operator()(int v) { value = v; }
    template<typename TO>
    explicit operator TO() const { return static_cast<TO>((*this)()); }  
    decltype(auto) operator==(const Type& u){ return (*this)() == u();  }
protected:
    std::ostream& display(std::ostream& s) const { return s<<value;  }
private:
    friend std::ostream& operator<<(std::ostream& s, const Type& c) { return s<<c.value;  }
    T value;
};
