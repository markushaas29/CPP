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
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { return visit<true>(q); }
	virtual ReturnType Visit(Date& d) { return visit<true>(d); }
	virtual ReturnType Visit(IBAN& i) { return visit<false>(i); }
protected:
	using Base = PredicateVisitor<D,E>;
	PredicateVisitor(std::shared_ptr<IElement> v): value{std::move(v)} {}
private:
	friend std::ostream& operator<<(std::ostream& s, const PredicateVisitor& t) 	{ return s<<"Value: "<<(*t.value);	}
	template<bool EnableType, typename T>
	ReturnType visit(T& q) 
	{
		if constexpr (!EnableOperation && !EnableType)
			return false;
		else
			return visitImpl(q); 
	};
	template<typename T>
	ReturnType visitImpl(T& q) 
	{
		if(auto p = std::dynamic_pointer_cast<T>(value))
             return Derived::op(q,*p);
		return false; 
	};
	std::shared_ptr<IElement> value;
};

class EqualVisitor: public PredicateVisitor<EqualVisitor,true>
{
	friend class PredicateVisitor<EqualVisitor,true>;
	template<typename T> inline static bool op(T l,T r) { return l == r; } 
public:
	EqualVisitor(std::shared_ptr<IElement> v): Base{std::move(v)} {}
};

class LessVisitor: public PredicateVisitor<LessVisitor,false>
{
	friend class PredicateVisitor<LessVisitor,false>;
	template<typename T> inline static bool op(T l,T r) { return l < r; } 
public:
	LessVisitor(std::shared_ptr<IElement> v): Base{std::move(v)} {}
};
