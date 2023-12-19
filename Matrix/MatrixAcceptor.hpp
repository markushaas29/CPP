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
	friend M;
public:
	static constexpr uint Order = M::Order;
	//using DescriptorType = MatrixDescriptor<Order, typename LeftType::ElementType>;
	//using ResultType = Matrix<Order, DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixAcceptor";
	inline static constexpr Literal LiteralType{TypeIdentifier};
private:
    using VisitorType = TransferVisitor;
	using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
	//template<typename T, typename A>
    static decltype(auto) accept(const M* m)
    {
		std::vector<std::unique_ptr<IPredicateVisitor>> vip;
		vip.push_back(std::make_unique<EqualVisitor>(std::make_unique<Quantity<Sum>>(-48)));
		vip.push_back(std::make_unique<EqualVisitor>(std::make_unique<Quantity<Sum>>(83.94)));
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(m);
		std::vector<std::shared_ptr<IElement>> temp;
        auto mat = mv.Collector().Create(vip.cbegin());    
		return apply(&mat,vip.cbegin()+1, vip.cend());
    }
	template<typename It>
    static decltype(auto) apply(const M* m, It begin, It end)
    {
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(m);
		std::cout<<"ACCCEPT"<<mv<<std::endl;
		std::vector<std::shared_ptr<IElement>> temp;
        auto mat = mv.Collector().Create(std::move(begin));    
		if(begin==end)
			return mat;
		return apply(&mat,begin+1,end);

    }
};
