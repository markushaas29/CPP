#include "Visitor.hpp"
#include <numeric>
#include <cmath>
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

class ICollectorVisitor
{
public:
	virtual std::shared_ptr<IElement> operator()() = 0;
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) = 0;
};

template<typename D, typename F, typename T, typename R=T>
class CollectorVisitor: virtual public BaseVisitor, virtual public Visitor<T>, virtual public ICollectorVisitor
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
//class ConsumptionVisitor: public CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>, public CollectorVisitor<ConsumptionVisitor<Date>,Diff<Date>,Date>
class ConsumptionVisitor: public CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>, public Visitor<Date>
{
	using Base = CollectorVisitor<ConsumptionVisitor<T>,Diff<T>,T>;
	using DBase = Visitor<Date>;
	using Ds = Quantity<Unit<0, 0, 0, 1>, DaysBase<1>, unsigned int>;
	struct Data
	{
		T quantity;
		Date date;
		Data(const Date& d): date{d} {};
	};
public:
	virtual Visitor<Date>::ReturnType Visit(Date& t) { elements.push_back(Data{t}); };
	virtual typename Base::ReturnType Visit(T& t) 
	{ 
		Base::Visit(t); 
		elements.back().quantity = t;
	};
	virtual std::shared_ptr<IElement> operator()(size_t i, size_t j) { return nullptr; };
	virtual std::shared_ptr<IElement> operator()() 
	{ 
		typename Base::IsT<Throwing>(Format("No elements of: ",T::Identifier, "!"))(0 < Base::func.Size());
		auto res = Base::func();

		std::sort (elements.begin(), elements.end(), [](const auto& l, const auto& r) { return l.date > r.date; });
		if(elements.begin()+1!=elements.end())
			return std::make_shared<T>(elements[0].quantity-elements[1].quantity);
		return std::make_shared<T>(elements[0].quantity);
	};
	virtual std::shared_ptr<IElement> operator()(Year y, Quantity<Time,Days,uint> days = Quantity<Time,Days,uint>{365}) 
	{ 
		std::sort (elements.begin(), elements.end(), [](const auto& l, const auto& r) { return l.date > r.date; });
		auto it = find_if(elements.begin(), elements.end(), [&y](const auto& l) { return l.date == y; });

		if(it!=elements.end())
		{
			std::vector<Data> preYear;
			std::copy_if(elements.begin(), elements.end(), std::back_inserter(preYear), [&](auto& d) { return d.date == (days < Quantity<Time,Days,uint>{365} ? y : y.Prev()); 	});
	    	auto itp = std::min_element(preYear.begin(), preYear.end(), [it,days] (auto a, auto b) {  return std::abs((int)((it->date-a.date)-days)) < std::abs((int)((it->date-b.date)-days)); });
			
			auto sd = Date(27,12,2019);
			auto ed = Date(15,12,2022);
			auto fs = Base::func();
			auto f = Base::Result().Values();
			auto pos = find_if(elements.begin(), elements.end(), [&] (auto s) { return s.quantity == T{0}; } );
			if(pos != elements.end() && (pos-elements.begin()) != 0 && (pos-elements.begin()) != (elements.size()-1)) 
			{
			for(auto e : elements)
				std::cout<<"Date: "<<e.date<<"Q: "<<e.quantity<<std::endl;
				size_t  p  = (pos-elements.begin());
				std::cout<<"PVal "<<f[p]<<std::endl;
	//			if (std::is_sorted(elements.end(), elements.end()+p))
	//			{
	//				std::cout<<"SORTED "<<std::endl;
	//			}
				if (std::is_sorted(f.end()+p+1,f.begin()))
				{
					std::cout<<"SORTED 2"<<std::endl;
				}
				
				auto it = find_if(elements.begin(), elements.end(), [&] (auto s) { return s.date == sd; } );
				if(it != elements.end())
					std::cout<<"SDate "<<it->quantity<<std::endl;
				auto it2 = find_if(elements.begin(), elements.end(), [&] (auto s) { return s.date == ed; } );
				if(it2 != elements.end())
					std::cout<<"SDate "<<it2->quantity<<std::endl;
	
	
				size_t pStart  = (it-elements.begin());
				size_t pEnd  = (it2-elements.begin());
				std::cout<<"SDate "<<elements[pStart].quantity<<"SDate "<<elements[pEnd].quantity<<std::endl;
				std::cout<<"SDate "<<elements[p-1].quantity-elements[pStart].quantity<<"SDate "<<elements[pEnd].quantity<<std::endl;
	
				auto val = (elements[p-1].quantity-elements[pStart].quantity) + elements[pEnd].quantity;
				std::cout<<"SDate "<<val<<std::endl;
			}

			return std::make_shared<T>(it->quantity-itp->quantity);
		}

		return (*this)();
	};
	auto Times()
	{
		std::vector<Date> dates;
		std::for_each(elements.cbegin(), elements.cend(),[&](const auto& i) { dates.push_back(i.date); });
		return Diff(dates);
	}
	auto FuncValues(){	return Base::func.Values();	}
	auto Spans()
	{
		std::vector<Ds> result;
		auto diffs = Times()();
		std::for_each(diffs.cbegin(), diffs.cend(),[&](const auto& i) { result.push_back(i()); });
		return result;
	}
	auto Diffs() { return Base::func(); }
	auto Values()
	{
		auto fs = Base::func();
		std::vector<T> result;
		std::for_each(fs.cbegin(), fs.cend(),[&](const auto& i) { 
				result.push_back(i()); });
		return result;
	}
	auto Averages()
	{
		auto values = Values();
		auto times = Spans();
		std::vector<decltype(values[0]/times[0])> result;
		for(size_t i = 0; i < values.size(); ++i)
			result.push_back(values[i]-times[i]);
		return result;
	}
	inline static std::string Identifier = std::string("Consumption") + T::Identifier;
private:
	auto closest(auto value, const auto& cont) const
	{
//	    //assert(!mV.empty());
//	    auto it = std::min_element(mV.begin(), mV.end(), [value] (double a, double b) {
//	        return std::abs(value - a) < std::abs(value - b);
//	    });
//	    //assert(it != mV.end());
//	
//	    return *it;
	}
	virtual std::ostream& display(std::ostream& s) const 
	{ 	
		auto res = Base::func();
		for(auto i = 0; i < res.size(); ++i)
			s<<res[i]<<" "<<res[i]()<<"\t from: "<<elements[i+1].date<<"\t to:"<<elements[i].date<<std::endl;
		return s;	
	};
	std::vector<Data> elements;
};
