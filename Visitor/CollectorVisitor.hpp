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
