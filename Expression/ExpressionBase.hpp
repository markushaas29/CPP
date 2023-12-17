#include <memory>
#pragma once

class IExpression
{
public:
	virtual std::shared_ptr<IExpression> Clone() = 0;
	virtual bool operator()() const = 0;
private:
	friend bool operator==(std::shared_ptr<IExpression> e1, std::shared_ptr<IExpression> e2) { std::cout<<"COM"<<std::endl; return (*e1)()==(*e2)(); };
};
