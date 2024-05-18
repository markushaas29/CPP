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
	virtual FuncType Result() { return func; };
	virtual std::shared_ptr<IElement> operator()() = 0;
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) = 0;
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<Derived>(); };
	size_t Size() { return func.Size(); }
private:
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const CollectorVisitor& t) { 	return t.display(s);	}
	virtual std::ostream& display(std::ostream& s) const { 	return s<<func;	};
	FuncType func;
};

template<typename T=Quantity<Sum,Pure,double>>
class AccumulationVisitor: public CollectorVisitor<AccumulationVisitor<T>,Acc<T>,T>
{
	using Base = CollectorVisitor<AccumulationVisitor<T>,Acc<T>, T>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) { return std::make_shared<typename Base::Type>(Base::func(i,j)); };
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0, Base::func.Size()); };
	inline static std::string Identifier = std::string("Accumulation") + T::Identifier;
};

template<typename T>
class DifferenceVisitor: public CollectorVisitor<DifferenceVisitor<T>,Diff<T>,T>
{
	using Base = CollectorVisitor<DifferenceVisitor<T>,Diff<T>, T>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j){ return std::make_shared<T>(Base::func()[0]);	};
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0,Base::func.Size()); };
	inline static std::string Identifier = std::string("Difference") + T::Identifier;
};

template<>
class DifferenceVisitor<Date>: public CollectorVisitor<DifferenceVisitor<Date>,Diff<Date>,Date, Quantity<Time,Days, uint>>
{
	using Base = CollectorVisitor<DifferenceVisitor<Date>,Diff<Date>, Date, Quantity<Time,Days,uint>>;
public:
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) {	return std::make_shared<Quantity<Time,Days,uint>>(Base::func()[i]);	};
	virtual std::shared_ptr<IElement> operator()() { return (*this)(0,Base::func.Size()); };
	inline static std::string Identifier = std::string("Difference") + Date::Identifier;
};

template<typename T>
class ConsumptionVisitor: public CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>, public Visitor<Date>
{
	using Base = CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>;
	using DBase = Visitor<Date>;
	using Ds = Quantity<Unit<0, 0, 0, 1>, DaysBase<1>, unsigned int>;
public:
	virtual Visitor<Date>::ReturnType Visit(Date& t) { elements.push_back(t); };
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) { return nullptr; };
	virtual std::shared_ptr<IElement> operator()() 
	{ 
		typename Base::IsT<Throwing>(Format("No elements of: ",T::Identifier, "!"))(0 < Base::func.Size());
		auto res = Base::func();
		for(size_t j = 0; j < elements.size(); ++j)
			std::cout<<"EL "<<j<<": "<<elements[j]<<std::endl;
		for(size_t j = 1; j < elements.size(); ++j)
			if((elements[0] - elements[j]) > Ds(320) && (elements[0] - elements[j]) < Ds(400))
				return std::make_shared<T>(res[j-1 >= 0 ? j-1 : 0]);
		return std::make_shared<T>(res[0]);
	};
	inline static std::string Identifier = std::string("Consumption") + T::Identifier;
private:
	virtual std::ostream& display(std::ostream& s) const 
	{ 	
		auto res = Base::func();
		for(auto i = 0; i < res.size(); ++i)
			s<<res[i]<<" "<<res[i]()<<"\t from: "<<elements[i]<<"\t to:"<<elements[i+1]<<std::endl;
		return s;	
	};
	std::vector<Date> elements;
};
