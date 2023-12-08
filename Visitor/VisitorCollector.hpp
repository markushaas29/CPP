 #include <vector>
 #include <memory>
 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"

#pragma once 
 
template<typename, typename> class M3;
template<typename T>
class Unique
{
public:
	template<typename P>
	auto operator()(const std::vector<P>& v) const
	{
		std::vector<T> result;
		std::for_each(v.cbegin(), v.cend(), [&](auto& vis) 
			{ 
				if(std::find_if(result.begin(), result.end(), [&](auto l) { return l == vis.template To<T>(); } ) == result.end())
					result.push_back(vis.template To<T>()); 
			});
		return result;
	}
};

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
	template<typename P, typename F>        
	decltype(auto) All(F f) const 
	{     
		if constexpr (std::is_same_v<P,IBAN> || std::is_same_v<P,typename VisitorType::SumType> || std::is_same_v<P,Date>)         
			return foreach<P>(f);
	}
	decltype(auto) Total()  const
	{
		Quantity<Sum> s;
		std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) { s = s + v.template To<typename VisitorType::SumType>(); });
		return s; 
	}
	template<typename P, typename F, typename FA>                      
	decltype(auto) Sort(F f, FA fa) const                      
	{
		using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
//	    auto values = All<P>(fa);   
	    auto values2 = Unique<P>()(*visitors);   

		std::cout<<"UNIQUE "<<values2.size()<<std::endl;
		std::vector<MT> res;
//	    std::for_each(values.cbegin(), values.cend(), [&](const auto& x)          
//	    {    
//	        std::vector<std::shared_ptr<IElement>> temp;                
//			std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) 
//	        {       
//	            if(f(v,x))                                                                        
//	            {                                      
//	               auto e = v.Create();    
//	               std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
//	            }                                   
//	        });      
//	                   
//	        MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
//	    	res.push_back(MT(md,temp));    
//	    });    
//		return M3<std::shared_ptr<IElement>,MatrixDescriptor<2,std::shared_ptr<IElement>>>(res);
		return values2;
    }
private:
	template<typename P, typename F>        
	decltype(auto) foreach(F f) const
	{
		std::vector<P> result;
		std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) 
			{ 
				if(std::find_if(result.begin(), result.end(), f) == result.end())
					result.push_back(v.template To<P>()); 
			});
		return result;
	}
};
