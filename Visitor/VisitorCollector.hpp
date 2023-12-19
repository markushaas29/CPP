 #include <vector>
 #include <memory>
 #include "Visitor.hpp"
 #include "ElementVisitor.hpp"
 #include "VisitorOperations.hpp"
 #include "PredicateVisitor.hpp"
 #include "../Expression/Expression.hpp"

#pragma once 
 
template<typename, typename> class M3;

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
	template<typename P>                      
	decltype(auto) Create(P p)  const
	{
		using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
	    std::vector<std::shared_ptr<IElement>> temp;                
		std::for_each(visitors->begin(), visitors->end(), [&](auto& v) 
	    {       
	        if(v.Is(**p))                                                                        
	        {                                      
	           auto e = v.Create();    
	           std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
	        }                                   
	    });     
	               
	    MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
	    return MT(md,temp);    
	}
	template<typename T>                      
	decltype(auto) Get(T t) const                      
	{
		using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
		std::vector<MT> res;
		auto p = EqualVisitor(std::make_unique<IBAN>("DE56600501017402051588"));
	    std::vector<std::shared_ptr<IElement>> temp;                
		std::for_each(visitors->begin(), visitors->end(), [&](auto& v) 
	    {       
	        if(v.Is(p))                                                                        
	        {                                      
	           auto e = v.Create();    
	           std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
	        }                                   
	    });      
	               
	    MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
	    res.push_back(MT(md,temp));    
		return M3<std::shared_ptr<IElement>,MatrixDescriptor<3,std::shared_ptr<IElement>>>(res);
    }
	template<typename P2, typename P1, typename F>                      
	decltype(auto) Sort(P1 pred, F fa) const                      
	{
		using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
	    auto values = fa(*visitors);   
		auto v1 = std::make_shared<Var<bool>>(false);
		auto v2 = std::make_shared<Var<bool>>(false);
		auto a = And(v1,v2);
		std::vector<MT> res;
	    std::for_each(values.cbegin(), values.cend(), [&](auto& x)          
	    {    
	        std::vector<std::shared_ptr<IElement>> temp;                
			std::for_each(visitors->begin(), visitors->end(), [&](auto& v) 
	        {       
				auto p = P2(std::make_unique<IBAN>(x));
	            (*v1)(v.Is(pred));
				(*v2)(v.Is(p));                                                                        
	            if(a())                                                                        
	            {                                      
	               auto e = v.Create();    
	               std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
	            }                                   
	        });      
	                   
	        MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
	    	res.push_back(MT(md,temp));    
	    });    
		return M3<std::shared_ptr<IElement>,MatrixDescriptor<3,std::shared_ptr<IElement>>>(res);
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
