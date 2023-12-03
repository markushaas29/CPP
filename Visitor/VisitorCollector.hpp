 #include <vector>
 #include <memory>
 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"

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
	template<typename P>        
     decltype(auto) All() const 
     {     
		std::vector<P> result;
		if constexpr (std::is_same_v<P,IBAN>)         
			std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) 
					{ 
						if(std::find(result.begin(), result.end(), v.IBANQ()) == result.end())
							result.push_back(v.IBANQ()); 
						});
		return result; 
     }
	decltype(auto) IBANS()  const 
	{ 
		typename TransferVisitor::PtrType result;
		std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) 
				{ 
					auto ptr = v.Create();
					std::for_each(ptr.cbegin(), ptr.cend(), [&](auto& ie) { result.push_back(ie); }); 
				});
		return result; 
	}
	decltype(auto) Total()  const
	{
		Quantity<Sum> s;
		std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) { s = s + v.SumQ(); });
		return s; 
	}
    template<typename P>                             
    decltype(auto) Sort() const                      
    {
		using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
        auto values = All<P>();    
		std::vector<MT> res;
        std::for_each(values.cbegin(), values.cend(), [&](const auto& x)          
        {    
            std::vector<std::shared_ptr<IElement>> temp;                
			std::for_each(visitors->cbegin(), visitors->cend(), [&](auto& v) 
                     {       
                         if(v.IBANQ() == x)                                                                        
                         {                                      
                            auto e = v.Create();    
                            std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
                         }                                   
                       });      
                                
            MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
        	res.push_back(MT(md,temp));    
        });    
    	return M3<std::shared_ptr<IElement>,MatrixDescriptor<2,std::shared_ptr<IElement>>>(res);
    }
private:
};
