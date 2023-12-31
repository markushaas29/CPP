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
class Entry;
class IElement;
template<typename,typename, typename> class Quantity;

class IPredicateVisitor: public VariadicVisitor<bool, Quantity<Sum,Pure,double>, Date, IBAN, Entry>
{
public:
	using ReturnType = bool;
	using ElementType = bool;
	virtual std::unique_ptr<IPredicateVisitor> Clone() = 0;
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) = 0;
	virtual ReturnType Visit(Date& d) = 0;
	virtual ReturnType Visit(IBAN& i) = 0;
	virtual ReturnType Visit(Entry& i) = 0;
	virtual std::ostream& Display(std::ostream& s) = 0;
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
	virtual ReturnType Visit(Entry& i) { 	return visit<false>(i); }
	virtual std::unique_ptr<IPredicateVisitor> Clone() { return std::make_unique<Derived>(value->Clone()); };
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<Derived>(value->Clone()); };

	static std::unique_ptr<IPredicateVisitor> Make(std::unique_ptr<IElement> e) { return std::make_unique<Derived>(std::move(e)); };
	virtual std::ostream& Display(std::ostream& s) { return s<<"Value: "<<(*value);	};
protected:
	inline static const std::string Identifier =  "Visitor";
	using Base = PredicateVisitor<D,E>;
	PredicateVisitor(std::unique_ptr<IElement> v): value{std::move(v)} {}
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
		if constexpr (std::is_same_v<T,Date>)
		{
			if(auto p = Cast::dynamic_unique_ptr<T>(value->Clone()))
        		return Derived::op(q,*p);
			if(auto p = Cast::dynamic_unique_ptr<Year>(value->Clone()))
        		return Derived::op(q.Y(),*p);
			if(auto p = Cast::dynamic_unique_ptr<Month>(value->Clone()))
        		return Derived::op(q.M(),*p);
			if(auto p = Cast::dynamic_unique_ptr<Day>(value->Clone()))
        		return Derived::op(q.D(),*p);
		}
		else
		{
			if(auto p = Cast::dynamic_unique_ptr<T>(value->Clone()))
        		return Derived::op(q,*p);
		}
		return false; 
	};
	std::unique_ptr<IElement> value;
};

class EqualVisitor: public PredicateVisitor<EqualVisitor,true>
{
	friend class PredicateVisitor<EqualVisitor,true>;
	template<typename T> inline static bool op(T l,T r) {	return l == r; } 
public:
	inline static const std::string Identifier = "Equal" + Base::Identifier;
	EqualVisitor(std::unique_ptr<IElement> v = nullptr): Base{std::move(v)} {}
};

class LessVisitor: public PredicateVisitor<LessVisitor,false>
{
	friend class PredicateVisitor<LessVisitor,false>;
	template<typename T> inline static bool op(T l,T r) { return l < r; } 
public:
	inline static const std::string Identifier = "Less" + Base::Identifier;
	LessVisitor(std::unique_ptr<IElement> v = nullptr): Base{std::move(v)} {}
};

template<typename D, bool E = false>
class BinaryVisitor: public IPredicateVisitor
{
	using Derived = D;
	inline static constexpr bool EnableOperation = E;
public:
	inline static const std::string Identifier = "Visitor";
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { return visit<true>(q); }
	virtual ReturnType Visit(Date& d) { return visit<true>(d); }
	virtual ReturnType Visit(IBAN& i) { return visit<false>(i); }
	static std::unique_ptr<IPredicateVisitor> Make(std::unique_ptr<IElement> l, std::unique_ptr<IElement> r) { return std::make_unique<Derived>(std::move(l), std::move(r)); };
protected:
	using Base = BinaryVisitor<D,E>;
	BinaryVisitor(std::unique_ptr<IPredicateVisitor> p1, std::unique_ptr<IPredicateVisitor> p2): v1{std::move(p1)}, v2{std::move(p2)} {}
private:
	//friend std::ostream& operator<<(std::ostream& s, const BinaryVisitor& t) 	{ return s<<"Value: "<<(*t.v1)<<" "<<(*t.v2);	}
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
		//if(auto p = Cast::dynamic_unique_ptr<T>(value->Clone()))
        //	return Derived::op(q,*p);
		return false; 
	};
	std::unique_ptr<IPredicateVisitor> v1;
	std::unique_ptr<IPredicateVisitor> v2;
};

class AndVisitor: public BinaryVisitor<AndVisitor,false>
{
	friend class BinaryVisitor<AndVisitor,false>;
	template<typename T> inline static bool op(T l,T r) { return l && r; } 
public:
	inline static const std::string Identifier = "And" + Base::Identifier;
	AndVisitor(std::unique_ptr<IPredicateVisitor> p1 = nullptr, std::unique_ptr<IPredicateVisitor> p2 = nullptr): Base{std::move(p1), std::move(p2)} {}
};
