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
	//template<typename T, typename A>
    static decltype(auto) accept(const M* m)
    {
		std::vector<std::unique_ptr<IPredicateVisitor>> vip;
		vip.push_back(std::make_unique<LessVisitor>(std::make_unique<Quantity<Sum>>(-40)));
		vip.push_back(std::make_unique<LessVisitor>(std::make_unique<Quantity<Sum>>(880)));
		auto mv = MatrixVisitor<TransferVisitor>();
		std::cout<<"ACCCEPT"<<std::endl;
		return apply(m,mv,vip.cbegin(), vip.cend());
    }
	template<typename R, typename It>
    static decltype(auto) apply(const M* m, R r, It begin, It end)
    {
		r.Visit(m);
		std::cout<<"APPLY"<<r<<std::endl;
		if(begin==end)
			return r;
		return apply(m,r,begin+1,end);

    }
};
