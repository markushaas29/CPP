 #include "Visitor.hpp"
 #include "../CSV/Element.hpp"
 #include "../CSV/Elements.hpp"
 #include "../Common/Date.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Quantity/Quantity.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 
class Date;
class IBAN;
class IElement;

template<typename,typename, typename> class Quantity;

class ElementVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>
{
	using ReturnType = void;
public:
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) {  };
	virtual ReturnType Visit(Date& q) {  };
};

class TransferVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>, public Visitor<IBAN>
{
	using ReturnType = void;
public:
	auto SumQ() { return sum;}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { sum = sum + q; };
	virtual ReturnType Visit(Date& d) { date = d;  };
	virtual ReturnType Visit(IBAN& i) { iban = i; };
private:
	Quantity<Sum,Pure,double> sum;
	IBAN iban;
	Date date;
	friend std::ostream& operator<<(std::ostream& s, const TransferVisitor& t) 	{ return s<<"IBAN: "<<t.iban<<"Date: "<<t.date<<"Sum: "<<t.sum;	}
};
