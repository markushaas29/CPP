 #include "../Unit/Unit.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 
class Date;
class IElement;
template<typename,typename, typename> class Quantity;

class BaseVisitor
{
public:
	virtual ~BaseVisitor(){}
};

template<class T, typename R = void>
class Visitor
{
public:
	using ReturnType = R;
	virtual ReturnType Visit(T&) = 0;
};

class ElementVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>
{
	using ReturnType = void;
public:
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { std::cout<<"I"<<std::endl; };
	virtual ReturnType Visit(Date& q) { std::cout<<"Q"<<std::endl; };
};

template<class T, typename R = void>
class ConstVisitor
{
public:
	using ReturnType = R;
	virtual ReturnType Visit(const T&) = 0;
};
	
template<typename R = void>
class BaseVisitable
{
public:
	using ReturnType = R;
	virtual ~BaseVisitable(){}
	virtual ReturnType Accept(BaseVisitor&) = 0;
//protected:
	template<class T>
	static ReturnType AcceptImpl(T& visited, BaseVisitor& visitor) 
	{
		if(Visitor<T,void>* p = dynamic_cast<Visitor<T,void>*>(&visitor))     
        {                                                
               std::cout<<"CAST T"<<std::endl;        
        }
		if(Visitor<Date,void>* p = dynamic_cast<Visitor<Date,void>*>(&visitor))     
        {                                                
               std::cout<<"CAST DATE"<<std::endl;        
        }
		if(Visitor<T,void>* p = dynamic_cast<Visitor<T,void>*>(&visitor))
			return p->Visit(visited);
		
		std::cout<<"ACCEPT"<<std::endl;
		return ReturnType();
	}		
	
	template<class T>
	static ReturnType AcceptConstImpl(const T& visited, BaseVisitor& visitor) 
	{
		//~ std::cout<<"Accept"<<std::endl;
		
		//~ if(ConstVisitor<FS::DirectoryInfo,R>* p = dynamic_cast<ConstVisitor<FS::DirectoryInfo,R>*>(&visitor))
			//~ std::cout<<"Accept Directory"<<std::endl;
		
		//~ if(ConstVisitor<FS::FileInfo,R>* p = dynamic_cast<ConstVisitor<FS::FileInfo,R>*>(&visitor))
		//~ {
			//~ std::cout<<"Accept File"<<std::endl;
			//~ p->Visit(visited);
		//~ }
		
		//~ if(ConstVisitor<T,R>* p = dynamic_cast<ConstVisitor<T,R>*>(&visitor))
			//~ return p->Visit(visited);
		
		return ReturnType();
	}		
};
	
#define DEFINE_VISITABLE() virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }
#define DEFINE_CONSTVISITABLE() virtual ReturnType AcceptConst(BaseVisitor& visitor) const { return AcceptConstImpl(*this, visitor); }
