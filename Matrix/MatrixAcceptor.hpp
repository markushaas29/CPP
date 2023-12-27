#include <memory>
#include <iostream>
#include <tuple>
#include <vector>
#include "MatrixDescriptor.hpp"
#include "MatrixOperations.hpp"
#include "MatrixVisitor.hpp"
#include "../Functional/Functional.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Visitor/PredicateVisitor.hpp"    
#include "../Visitor/ElementVisitor.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#pragma once

template<std::size_t N, typename DT> class Matrix;

template<typename M>
class MatrixAcceptor 
{
    using VisitorType = TransferVisitor;
	struct Data
	{
		VisitorType visitor;
        std::vector<std::shared_ptr<IElement>> elements;                
	};
	friend M;
public:
	static constexpr uint Order = M::Order;
	//using DescriptorType = MatrixDescriptor<Order, typename LeftType::ElementType>;
	//using ResultType = Matrix<Order, DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixAcceptor";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	const M& Get() { return matrix; }
	MatrixAcceptor(): visitors{std::make_unique<std::vector<VisitorType>>()}, data{std::make_unique<std::vector<Data>>()} { }
private:
	M matrix;
	std::unique_ptr<std::vector<VisitorType>> visitors;
	std::unique_ptr<std::vector<Data>> data;
	using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
	template<typename T>
    decltype(auto) pipe(const M* m, std::unique_ptr<T> p) { return pipe(m, std::shared_ptr<T>(std::move(p)));}
	
	template<typename T>
    decltype(auto) pipe(const M* m, std::shared_ptr<T> p)
    {
		std::vector<std::shared_ptr<T>> v;
		v.push_back(p);
		return accept(m,v);
    }
	
	template<typename T>
    decltype(auto) accept(const M* m, const T& v)
    {
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(m);
		visit(m);
		auto mc = create(v.cbegin());
//        std::for_each(visitors->begin(), visitors->end(), [&](auto& v) { std::cout<<v<<std::endl; }); 

		return apply(mv.Collector().Create(v.cbegin()),v.cbegin()+1, v.cend());
    }
	
	template<typename It>
    decltype(auto) apply(M&& m, It begin, It end)
    {
		if(begin==end)
			return M(m.descriptor, *m.elements);
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(&m);
		return apply(mv.Collector().Create(std::move(begin)),begin+1,end);

    }

	template<typename MT>
	decltype(auto) visit(MT* m) const
    {
        if constexpr (MT::Order==1)
        {
            VisitorType v;
        	std::vector<std::shared_ptr<IElement>> elements;                
            for(auto i=0; i<m->Rows(); ++i)
			{
                (*m->elements->at(i))->Accept(v);
                elements.push_back((*m->elements->at(i)));
			}
			visitors->push_back(v);
            data->push_back({v,elements});
        }
        else
        {
            MatrixVisitor<VisitorType> v;
            for(auto i = 0; i != m->Rows(); ++i)
			{
				auto x = (*m)[i];
				visit(&x);
			}
        }
    };

    template<typename P>                      
    decltype(auto) create(P p)  const
    {
        using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
        std::vector<std::shared_ptr<IElement>> temp;                
        std::for_each(data->begin(), data->end(), [&](auto& d) 
        {       
            if(d.visitor.Is(**p))                                                                        
            {                                      
               auto e = d.elements;    
               std::for_each(e.cbegin(), e.cend(), [&](const auto& v) { temp.push_back(v); });             
            }                                   
        });     
                   
        MatrixDescriptor<2,std::shared_ptr<IElement>> md{{temp.size()/VisitorType::Order,VisitorType::Order}};    
        return MT(md,temp);    
    }
};
