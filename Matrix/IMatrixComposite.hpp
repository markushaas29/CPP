#include <memory>
#include <tuple>
#include <vector>
#include "MatrixCompositeResult.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Visitor/CollectorVisitor.hpp"

#pragma once

template<std::size_t, typename> class Matrix;

template<typename T, typename Q> class MatrixComposition;
template<typename T, typename Q>
class IMatrixComposite
{
public:
	using MatrixType = T;
	using QuantityType = Q;
	using ElementType = T::ElementType;
	using PredicateType = std::unique_ptr<IPredicateVisitor>;
	using VisitorType = std::unique_ptr<BaseVisitor>;
	using ResultMatrixType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
	using ResultType = std::unique_ptr<IResult<QuantityType,ResultMatrixType>>;
	virtual ResultType operator()(T& m) const = 0;
	virtual std::string_view Name() const = 0;
	virtual size_t Size() const = 0;
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixComposite& m) { return m.display(s); }
	virtual std::ostream& display(std::ostream& s, size_t i = 0) const = 0;
private:
};


template<template<typename, typename> class D, typename T, typename Q = Quantity<Sum>>
class MatrixCompositeBase: public IMatrixComposite<T,Q>
{
protected:
	using Base = IMatrixComposite<T,Q>;
	using Derived = D<T,Q>;
	friend class D<T,Q>;
	using ResultType = typename Base::ResultType;
	using ResultMatrixType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
	using UnitVectorType = FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposition";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixCompositeBase(const std::string& n):  name{n} {}
	virtual std::string_view Name() const { return name; };
private:
	std::string name;
	static std::ostream& in(std::ostream& s, size_t add) 
	{
		for(auto i=0; i<add; ++i)
			s<<"  ";
		return s;
	}
};
