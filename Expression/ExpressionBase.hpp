#include <memory>
#include <iostream>
#pragma once

class IExpression
{
public:
	virtual std::shared_ptr<IExpression> Clone() = 0;
	virtual bool operator()() const = 0;
protected:
	virtual std::ostream& display(std::ostream& s) const = 0;
private:
	friend std::ostream& operator<<(std::ostream& s, const IExpression& c) 
	{ 
		s<<"{"<<std::boolalpha;
		c.display(s);
		return s<<"}";  
	}
	friend bool operator==(std::shared_ptr<IExpression> e1, std::shared_ptr<IExpression> e2) { std::cout<<"COM"<<std::endl; return (*e1)()==(*e2)(); };
};
