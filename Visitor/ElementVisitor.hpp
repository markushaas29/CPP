 #include "Visitor.hpp"
 #include "../CSV/Element.hpp"
 #include "../CSV/Elements.hpp"
 #include "../Common/Date.hpp"
 #include "../Common/UniqueCast.hpp"
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

class PredicateVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>,bool>//, public Visitor<Date>
{
	using ReturnType = bool;
public:
	PredicateVisitor(std::unique_ptr<IElement> v): value{std::move(v)} {}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) 
	{
		//auto temp = std::make_unique<IElement>(*value);
		if(auto p = Cast::dynamic_unique_ptr<Quantity<Sum,Pure,double>>(std::move(value)))
		{
			 value = std::make_unique<Quantity<Sum,Pure,double>>(*p);
             return q == *p;
		}
		return false; };
	//virtual ReturnType Visit(Date& q) {  };
private:
	std::unique_ptr<IElement> value;
};

class TransferVisitor: public BaseVisitor, public Visitor<Quantity<Sum,Pure,double>>, public Visitor<Date>, public Visitor<IBAN>
{
	using ReturnType = void;
public:
	using PtrType = std::vector<std::shared_ptr<IElement>>;
	using SumType = Quantity<Sum,Pure,double>;
	inline static constexpr size_t Order = 3;
	auto I() const { return iban; }
	template<typename T>
	auto To() const
	{
		if constexpr (std::is_same_v<T,Quantity<Sum,Pure,double>>)
			return sum;
		else if constexpr (std::is_same_v<T,IBAN>)
			return iban;
		else if constexpr (std::is_same_v<T,Date>)
			return date;
	}
	auto Create() const	{	return std::vector<std::shared_ptr<IElement>> {std::make_shared<IBAN>(iban), std::make_shared<Quantity<Sum,Pure,double>>(sum), std::make_shared<Date>(date.D(), date.M(), date.Y()) };	}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { sum = sum + q; };
	virtual ReturnType Visit(Date& d) { date = d;  };
	virtual ReturnType Visit(IBAN& i) { iban = i; };
private:
	SumType sum;
	IBAN iban;
	Date date;
	friend std::ostream& operator<<(std::ostream& s, const TransferVisitor& t) 	{ return s<<"IBAN: "<<t.iban<<"Date: "<<t.date<<"Sum: "<<t.sum;	}
};
