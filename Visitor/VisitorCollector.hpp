 #include <vector>
 #include <memory>
 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"

#pragma once 

template<typename T, template<typename> class D>
class BaseVisitorCollector
{
	friend class D<T>;
public:
	using VisitorType = T;
	using Derived = D<T>;
	decltype(auto) Add(const VisitorType& v) const { visitors->push_back(v); }
protected:
	BaseVisitorCollector(): visitors(std::make_unique<std::vector<VisitorType>>()) { }
	BaseVisitorCollector(const BaseVisitorCollector& b): visitors(std::make_unique<std::vector<VisitorType>>(b.visitors->cbegin(), b.visitors->cend())) { }
private:
	friend std::ostream& operator<<(std::ostream& s, const BaseVisitorCollector& t)  
	{
		std::for_each(t.visitors->cbegin(), t.visitors->cend(), [&](auto& v) { s<<v<<"\n"; });
		return s; 
	}
	std::unique_ptr<std::vector<VisitorType>> visitors;
};

template<typename T>
class VisitorCollector: public BaseVisitorCollector<T,VisitorCollector>
{
public:
	using VisitorType = T;
	using Base = BaseVisitorCollector<T,VisitorCollector>;
	VisitorCollector() { }
private:
};

template<>
class VisitorCollector<TransferVisitor>: public BaseVisitorCollector<TransferVisitor,VisitorCollector>
{
public:
	using VisitorType = TransferVisitor;
	using Base = BaseVisitorCollector<TransferVisitor,VisitorCollector>;
	VisitorCollector() { }
	decltype(auto) Total()  const
	{
		Quantity<Sum> s;
		std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) { s = s + v.SumQ(); });
		return s; 
	}
private:
};
