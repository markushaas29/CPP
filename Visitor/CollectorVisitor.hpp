#include "Visitor.hpp"
#include <numeric>
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

template<typename D, typename T, typename R=T>
class CollectorVisitor: public BaseVisitor, public Visitor<T>
{
	using Base = Visitor<T>;
	using Derived = D;
	using Type = T;
	friend D;
public:
	static std::unique_ptr<BaseVisitor> Make(const std::string& s) { return std::make_unique<Derived>();	}
	virtual typename Base::ReturnType Visit(T& t) { elements.push_back(t); };
	virtual R operator()(size_t i = 0, size_t j = 0) = 0;
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<Derived>(); };
	size_t Size() { return elements.size(); }
private:
	friend std::ostream& operator<<(std::ostream& s, const CollectorVisitor& t) 	
	{ 
		std::for_each(t.elements.cbegin(), t.elements.cend(), [&](const T& e) { s<<e<<"\n"; });
		return s;	
	}
	std::vector<T> elements;
};

class AccumulationVisitor: public CollectorVisitor<AccumulationVisitor, Quantity<Sum,Pure,double>>
{
	using Base = CollectorVisitor<AccumulationVisitor, Quantity<Sum,Pure,double>>;
public:
	virtual typename Base::Type operator()(size_t i = 0, size_t j = 0) { return std::accumulate(Base::elements.begin(), Base::elements.end(), typename Base::Type{0}); };
	inline static constexpr const char* Identifier = "Accumulation";
};

template<typename T>
class DifferenceVisitor: public CollectorVisitor<DifferenceVisitor<T>, T>
{
	using Base = CollectorVisitor<DifferenceVisitor<T>, T>;
public:
	virtual typename Base::Type operator()(size_t i = 0, size_t j = 0) 
	{ 
		std::vector<T> res;
		//std::adjacent_difference(Base::elements.begin(), Base::elements.end(), res.begin(), [&](const auto& l, const auto& r) { return Base::elements[0]; } ); I
		for(size_t i = 0; i < Base::elements.size()-1; ++i)
			res.push_back(Base::elements[i] - Base::elements[i+1]);
		return res[0];
	};
	inline static constexpr const char* Identifier = "Difference";
};

template<>
class DifferenceVisitor<Date>: public CollectorVisitor<DifferenceVisitor<Date>, Date, Quantity<Time,Days, uint>>
{
	using Base = CollectorVisitor<DifferenceVisitor<Date>, Date, Quantity<Time,Days,uint>>;
public:
	virtual Quantity<Time,Days,uint> operator()(size_t i = 0, size_t j = 0) 
	{ 
		std::vector<Quantity<Time,Days,uint>> res;
		//std::adjacent_difference(Base::elements.begin(), Base::elements.end(), res.begin(), [&](const auto& l, const auto& r) { return Base::elements[0]; } ); I
		for(size_t i = 0; i < Base::elements.size()-1; ++i)
			res.push_back(Base::elements[i] - Base::elements[i+1]);
		return res[i];
	};
	inline static constexpr const char* Identifier = "Difference";
};

template<typename T>
class ConsumptionVisitor: public CollectorVisitor<ConsumptionVisitor<T>,T>, public Visitor<Date>
{
	using Base = CollectorVisitor<ConsumptionVisitor<T>,T>;
	using Ds = Quantity<Unit<0, 0, 0, 1>, DaysBase<1>, unsigned int>;
public:
	virtual Visitor<Date>::ReturnType Visit(Date& t) { elements.push_back(t); };
	virtual typename Base::Type operator()(size_t i = 0, size_t j = 0) 
	{ 
		std::vector<T> res;
		for(size_t i = 1; i < Base::elements.size(); ++i)
			res.push_back(Base::elements[0] - Base::elements[i]);
		for(size_t j = 1; j < elements.size(); ++j)
			if((elements[0] - elements[j]) > Ds(320) && (elements[0] - elements[j]) < Ds(400))
				return res[j-1 >= 0 ? j-1 : 0];
		return res[0];
	};
	inline static constexpr const char* Identifier = "Consumption";
private:
	std::vector<Date> elements;
};
