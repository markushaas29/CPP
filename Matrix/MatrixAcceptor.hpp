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
	const M& Get() { return matrix; }
private:
	M matrix;
    using VisitorType = TransferVisitor;
	using MT = Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>;                                
	//template<typename T, typename A>
    static decltype(auto) accept(const M* m)
    {
		std::vector<std::shared_ptr<IPredicateVisitor>> vip;
		vip.push_back(std::make_shared<LessVisitor>(std::make_unique<Quantity<Sum>>(-30)));
		vip.push_back(std::make_shared<EqualVisitor>(std::make_unique<Quantity<Sum>>(-48)));
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(m);
		return apply(mv.Collector().Create(vip.cbegin()),vip.cbegin()+1, vip.cend());
    }
	template<typename It>
    static decltype(auto) apply(M&& m, It begin, It end)
    {
		if(begin==end)
			return M(m.descriptor, *m.elements);
		auto mv = MatrixVisitor<TransferVisitor>();
		mv.Visit(&m);
		return apply(mv.Collector().Create(std::move(begin)),begin+1,end);

    }
};
