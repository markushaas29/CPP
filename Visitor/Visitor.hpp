 #include "../Unit/Unit.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 

class BaseVisitor
{
public:
	template<typename T>
	T As() 
	{
		if(auto p = dynamic_cast<T*>(this))
			return *p;
		
		return T();
	}		
	virtual ~BaseVisitor(){}
};

template<class T, typename R = void>
class Visitor
{
public:
	using ReturnType = R;
	virtual ReturnType Visit(T&) = 0;
};

template<typename R, typename... T>
class VariadicVisitor: public BaseVisitor, public Visitor<T,R>... 
{
public:
	using ReturnType = R;
};
	
template<typename R = void>
class BaseVisitable
{
public:
	using ReturnType = R;
	virtual ~BaseVisitable(){}
	virtual ReturnType Accept(BaseVisitor&) = 0;
protected:
	template<class T>
	static ReturnType AcceptImpl(T& visited, BaseVisitor& visitor) 
	{
		if(Visitor<T,ReturnType>* p = dynamic_cast<Visitor<T,ReturnType>*>(&visitor))
			return p->Visit(visited);
		
		return ReturnType();
	}		
};

template<typename R = bool>
class BoolVisitable
{
public:
	using ReturnType = R;
	virtual ~BoolVisitable(){}
	virtual ReturnType Is(BaseVisitor&) = 0;
protected:
	template<class T>
	static ReturnType AcceptPredicate(T& visited, BaseVisitor& visitor) 
	{
		if(Visitor<T,ReturnType>* p = dynamic_cast<Visitor<T,ReturnType>*>(&visitor))
			return p->Visit(visited);
		
		return ReturnType();
	}		
};
	
#define DEFINE_VISITABLE() virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }
#define DEFINE_CONSTVISITABLE() virtual ReturnType AcceptConst(BaseVisitor& visitor) const { return AcceptConstImpl(*this, visitor); }
