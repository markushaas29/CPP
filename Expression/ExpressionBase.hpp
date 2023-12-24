#include <memory>
#include <iostream>
#pragma once

template<typename T=bool>
class IExpression
{
public:
	using Type = T;
	virtual std::shared_ptr<IExpression> Clone() = 0;
	virtual T operator()() const = 0;
protected:
	virtual std::ostream& display(std::ostream& s) const = 0;
private:
	friend std::ostream& operator<<(std::ostream& s, const IExpression& c) 
	{ 
		s<<"{";
		if constexpr (std::is_same_v<T,bool>)
			s<<std::boolalpha;
		c.display(s);
		return s<<"}";  
	}
	friend bool operator==(std::shared_ptr<IExpression> e1, std::shared_ptr<IExpression> e2) { return (*e1)()==(*e2)(); };
};