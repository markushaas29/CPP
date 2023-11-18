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
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) {  };
	virtual ReturnType Visit(Date& q) {  };
};

class TransferVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>
{
	using ReturnType = void;
public:
	auto SumQ() { return sum;}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { sum = sum + q; };
	virtual ReturnType Visit(Date& q) {  };
private:
	Quantity<Sum,Pure,double> sum;
};
