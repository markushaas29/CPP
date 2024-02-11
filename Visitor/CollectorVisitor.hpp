#include "Visitor.hpp"
#include <numeric>
#include "../CSV/Element.hpp"
#include "../CSV/Elements.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/Date.hpp"
#include "../Common/UniqueCast.hpp"
#include "../Unit/Unit.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once 

class Date;
class IBAN;
class IElement;
template<typename,typename, typename> class Quantity;

template<typename D, typename F, typename T, typename R=T>
class CollectorVisitor: public BaseVisitor, public Visitor<T>
{
	using Base = Visitor<T>;
	using Derived = D;
	using Type = T;
	using FuncType = F;
	friend D;
public:
	inline static constexpr const char TypeIdentifier[] = "CollectorVisitor";
	inline static constexpr Literal TypeId{TypeIdentifier};
	static std::unique_ptr<BaseVisitor> Make(const std::string& s) { return std::make_unique<Derived>();	}
	virtual typename Base::ReturnType Visit(T& t) { func.Push(t); };
	virtual std::shared_ptr<IElement> operator()() = 0;
	virtual FuncType Func() { return func; };
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) = 0;
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<Derived>(); };
	size_t Size() { return elements.size(); }
private:
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const CollectorVisitor& t) { 	return s<<t.func;	}
	std::vector<T> elements;
	FuncType func;
};

template<typename T=Quantity<Sum,Pure,double>>
class AccumulationVisitor: public CollectorVisitor<AccumulationVisitor<T>,Acc<T>,T>
{
	using Base = CollectorVisitor<AccumulationVisitor<T>,Acc<T>, T>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) { return std::make_shared<typename Base::Type>(Base::func(i,j)); };
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0, Base::elements.size()); };
	inline static constexpr const char* Identifier = "Accumulation";
};

template<typename T>
class DifferenceVisitor: public CollectorVisitor<DifferenceVisitor<T>,Diff<T>,T>
{
	using Base = CollectorVisitor<DifferenceVisitor<T>,Diff<T>, T>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j){ return std::make_shared<T>(Base::func()[0]);	};
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0,Base::elements.size()); };
	inline static constexpr const char* Identifier = "Difference";
};

template<>
class DifferenceVisitor<Date>: public CollectorVisitor<DifferenceVisitor<Date>,Diff<Date>,Date, Quantity<Time,Days, uint>>
{
	using Base = CollectorVisitor<DifferenceVisitor<Date>,Diff<Date>, Date, Quantity<Time,Days,uint>>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j)
	{ 
		std::vector<Quantity<Time,Days,uint>> res;
		for(size_t i = 0; i < Base::elements.size()-1; ++i)
			res.push_back(Base::elements[i] - Base::elements[i+1]);
		return std::make_shared<Quantity<Time,Days,uint>>(res[i]);
	};
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0,Base::elements.size()); };
	inline static constexpr const char* Identifier = "Difference";
};

template<typename T>
class ConsumptionVisitor: public CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>, public Visitor<Date>
{
	using Base = CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>;
	using Ds = Quantity<Unit<0, 0, 0, 1>, DaysBase<1>, unsigned int>;
public:
	virtual Visitor<Date>::ReturnType Visit(Date& t) { elements.push_back(t); };
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) { return nullptr; };
	virtual std::shared_ptr<IElement> operator()() 
	{ 
		std::vector<T> res;
		typename Base::IsT<Throwing>(Format("No elements of: ",T::Identifier, "!"))(0 < Base::elements.size());
		for(size_t i = 1; i < Base::elements.size(); ++i)
			res.push_back(Base::elements[0] - Base::elements[i]);
		for(size_t j = 1; j < elements.size(); ++j)
			if((elements[0] - elements[j]) > Ds(320) && (elements[0] - elements[j]) < Ds(400))
				return std::make_shared<T>(res[j-1 >= 0 ? j-1 : 0]);
		return std::make_shared<T>(res[0]);
	};
	inline static constexpr const char* Identifier = "Consumption";
private:
	std::vector<Date> elements;
};
