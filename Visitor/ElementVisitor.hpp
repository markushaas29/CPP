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
	virtual typename Base::Type operator()() { return std::accumulate(Base::elements.begin(), Base::elements.end(), typename Base::Type{0}); };
public:
	inline static constexpr const char* Identifier = "Accumulation";
};

template<typename... Types>
class TransferVisitor: public VariadicVisitor<void, Types...>, public BoolVisitable<bool>
{
	using ReturnType = void;
public:
	using PtrType = std::vector<std::shared_ptr<IElement>>;
	using SumType = Quantity<Sum,Pure,double>;
	inline static constexpr size_t Order = 4;
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
	auto Create() const	{	return std::vector<std::shared_ptr<IElement>> {std::make_shared<IBAN>(iban), std::make_shared<Quantity<Sum,Pure,double>>(sum), std::make_shared<Date>(date.D(), date.M(), date.Y()), std::make_shared<Entry>(entry) };	}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { sum = sum + q; };
	virtual ReturnType Visit(Date& d) { date = d;  };
	virtual ReturnType Visit(IBAN& i) { iban = i; };
	virtual ReturnType Visit(Entry& i) { entry = i; };
	virtual bool Is(BaseVisitor& visitor) 
	{
		std::vector<std::shared_ptr<IElement>> v = { std::make_shared<SumType>(sum), std::make_shared<IBAN>(iban), std::make_shared<Date>(date), std::make_shared<Entry>(entry) };
		for(auto p : v)
			if(p->Is(visitor))
				return true;
		return false; 
	};
private:
	SumType sum;
	IBAN iban;
	Date date;
	Entry entry;
	friend std::ostream& operator<<(std::ostream& s, const TransferVisitor& t) 	{ return s<<"IBAN: "<<t.iban<<"Date: "<<t.date<<"Sum: "<<t.sum;	}
};
