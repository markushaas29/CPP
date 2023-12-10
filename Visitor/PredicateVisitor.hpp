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

class IPredicateVisitor: public VariadicVisitor<bool, Quantity<Sum,Pure,double>, Date, IBAN>
{
public:
	using ReturnType = bool;
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) = 0;
	virtual ReturnType Visit(Date& d) = 0;
	virtual ReturnType Visit(IBAN& i) = 0;
};

template<typename D, bool E = false>
class PredicateVisitor: public IPredicateVisitor
{
	using Derived = D;
	inline static constexpr bool EnableOperation = E;
public:
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { return visit(q); }
	virtual ReturnType Visit(Date& d) { return visit(d); }
	virtual ReturnType Visit(IBAN& i) { return visit(i); }
protected:
	using Base = PredicateVisitor<D,E>;
	PredicateVisitor(std::unique_ptr<IElement> v): value{std::move(v)} {}
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

class EqualVisitor: public PredicateVisitor<EqualVisitor,true>
{
	friend class PredicateVisitor<EqualVisitor,true>;
public:
	EqualVisitor(std::unique_ptr<IElement> v): Base{std::move(v)} {}
private:
	//template<typename T>
};
