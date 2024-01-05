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

template<typename D, typename T>
class CollectorVisitor: public BaseVisitor, public Visitor<T>
{
	using Base = Visitor<T>;
	using Derived = D;
	using Type = T;
	friend D;
public:
	static std::unique_ptr<BaseVisitor> Make(const std::string& s) { return std::make_unique<Derived>();	}
	virtual typename Base::ReturnType Visit(T& t) { elements.push_back(t); };
	virtual T operator()() = 0;
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
	virtual typename Base::Type operator()() { return std::accumulate(Base::elements.begin(), Base::elements.end(), typename Base::Type{0}); };
	inline static constexpr const char* Identifier = "Accumulation";
};

template<typename T>
class DifferenceVisitor: public CollectorVisitor<DifferenceVisitor<T>, T>
{
	using Base = CollectorVisitor<DifferenceVisitor<T>, T>;
public:
	virtual typename Base::Type operator()() 
	{ 
		std::vector<T> res;
		std::cout<<"SIZE\n"<<Base::elements[0]<<std::endl;
		//std::adjacent_difference(Base::elements.begin(), Base::elements.end(), res.begin(), [&](const auto& l, const auto& r) { return Base::elements[0]; } ); I
		for(size_t i = 0; i < Base::elements.size()-1; ++i)
			res.push_back(Base::elements[i] - Base::elements[i+1]);
		std::cout<<"SIZE\n"<<res.size()<<std::endl;
		return res[0];
	};
	inline static constexpr const char* Identifier = "Difference";
};
