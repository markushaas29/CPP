 #include "Visitor.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 
class Date;
class IElement;

template<typename,typename, typename> class Quantity;

class ElementVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>
{
	using ReturnType = void;
public:
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { std::cout<<"I"<<std::endl; };
	virtual ReturnType Visit(Date& q) { std::cout<<"Q"<<std::endl; };
};
