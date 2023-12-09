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

class PredicateVisitor: public VariadicVisitor<bool, Quantity<Sum,Pure,double>, Date>
{
	using ReturnType = bool;
public:
	PredicateVisitor(std::unique_ptr<IElement> v): value{std::move(v)} {}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { return visit(q); }
	virtual ReturnType Visit(Date& d) { return visit(d); }
private:
	friend std::ostream& operator<<(std::ostream& s, const PredicateVisitor& t) 	{ return s<<"Value: "<<(*t.value);	}
	template<typename T>
	ReturnType visit(T& q) 
	{
		if(auto p = Cast::dynamic_unique_ptr<T>(std::move(value)))
		{
			 value = std::make_unique<T>(*p);
             return q == *p;
		}
		return false; 
	};
	std::unique_ptr<IElement> value;
};

class TransferVisitor: public VariadicVisitor<void, Quantity<Sum,Pure,double>, Date, IBAN>, public BoolVisitable<bool>
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
	virtual bool Is(BaseVisitor& visitor) 
	{
		std::vector<std::shared_ptr<IElement>> v = { std::make_shared<SumType>(sum), std::make_shared<IBAN>(iban), std::make_shared<Date>(date) };
		for(auto p : v)
			if(p->Is(visitor))
				return true;
		return false;// AcceptPredicate<D>(*dynamic_cast<D*>(this), visitor); 
	};
private:
	SumType sum;
	IBAN iban;
	Date date;
	friend std::ostream& operator<<(std::ostream& s, const TransferVisitor& t) 	{ return s<<"IBAN: "<<t.iban<<"Date: "<<t.date<<"Sum: "<<t.sum;	}
};
