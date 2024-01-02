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

template<typename T, typename Q>
class IMatrixComposite
{
public:
	using MatrixType = T;
	using QuantityType = Q;
	using ElementType = T::ElementType;
	using ResultMatrixType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
	using ResultType = MatrixCompositeResult<QuantityType,ResultMatrixType>;
	virtual ResultType operator()(T& m) const = 0;
	virtual std::string_view Name() const = 0;
	virtual size_t Size() const = 0;
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixComposite& m) { return m.display(s); }
private:
	virtual std::ostream& display(std::ostream& s) const = 0;
};


template<template<typename, typename> class D, typename T, typename Q = Quantity<Sum>>
class MatrixCompositeBase: public IMatrixComposite<T,Q>
{
protected:
	using Base = IMatrixComposite<T,Q>;
	using Derived = D<T,Q>;
	friend class D<T,Q>;
	using ResultType = typename Base::ResultType;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposition";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixCompositeBase(const std::string& n):  name{n} {}
	virtual std::string_view Name() const { return name; };
private:
	std::string name;
	//virtual std::ostream& display(std::ostream& s) const { return s<<static_cast<Derived>(*this); };
};
template<typename T, typename Q = Quantity<Sum>>
class MatrixComposition: public MatrixCompositeBase<MatrixComposition,T,Q>
{
	using Base = MatrixCompositeBase<MatrixComposition,T,Q>;
	using PredicateType = std::unique_ptr<IPredicateVisitor>;
	using VisitorType = std::unique_ptr<BaseVisitor>;
//	using ResultType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposition";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixComposition(std::unique_ptr<std::vector<PredicateType>> p, std::unique_ptr<std::vector<VisitorType>> v, const std::string& n): Base{n}, predicates{std::move(p)}, visitors{std::move(v)} {}
	virtual typename Base::ResultType operator()(Base::MatrixType& m) const
	{
		typename Base::ResultMatrixType result;
		if(predicates->size()>0)
		{
			result = m |  (predicates->at(0)->Clone());
			std::for_each(predicates->cbegin(), predicates->cend(), [&](const auto& i) { result = result | (i->Clone()); });
			auto cv =result.Accept(visitors->at(0)->Copy());
			return typename Base::ResultType((cv->template As<AccumulationVisitor>())(), result);
		}

		return typename Base::ResultType{Q{0}};
	}
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const 
	{ 
		std::unique_ptr<std::vector<PredicateType>> p = std::make_unique<std::vector<PredicateType>>();
		std::for_each(predicates->cbegin(), predicates->cend(), [&p](const auto& i) { p->push_back(i->Clone()); });
		std::unique_ptr<std::vector<VisitorType>> v= std::make_unique<std::vector<VisitorType>>();
		std::for_each(visitors->cbegin(), visitors->cend(), [&v](const auto& i) { v->push_back(i->Copy()); });
		return std::make_unique<MatrixComposition>(std::move(p),std::move(v),Base::name);
	}
	virtual size_t Size() const { return 1; };
private:
	std::unique_ptr<std::vector<PredicateType>> predicates;
	std::unique_ptr<std::vector<VisitorType>> visitors;
	virtual std::ostream& display(std::ostream& s) const { return s<<(*this); };
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixComposition& m) 
	{ 
		s<<"Name: "<<m.Name()<<std::endl;
		std::for_each(m.predicates->cbegin(), m.predicates->cend(), [&s](const auto& i) { s<<*i<<"\n"; });
		//std::for_each(m.visitors->cbegin(), m.visitors->cend(), [&s](const auto& i) { s<<*i<<"\n"; });
		return s;  
	}
};

template<typename T, typename Q = Quantity<Sum>>
class MatrixComposite: public MatrixCompositeBase<MatrixComposite,T,Q>
{
	using Base = MatrixCompositeBase<MatrixComposite,T,Q>;
	using DataType = std::unique_ptr<IMatrixComposite<T,Q>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposite";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixComposite(const std::string& n): Base{n}, composites{std::make_unique<std::vector<DataType>>()} {}
	MatrixComposite(const std::string& n, std::unique_ptr<std::vector<DataType>> c): Base{n}, composites{std::move(c)} {}
	MatrixComposite(const std::string& n, DataType c): Base{n}, composites{std::make_unique<std::vector<DataType>>()} {	composites->push_back(std::move(c)); }
	virtual typename Base::ResultType operator()(Base::MatrixType& m) const
	{
		Q result{0};
		std::for_each(composites->cbegin(), composites->cend(), [&](const auto& c)	{ 	result = result + ((*c)(m)).Value(); }); 
		return typename Base::ResultType{result};
	}
	virtual DataType Clone() const 
	{ 
		std::unique_ptr<std::vector<DataType>> c = std::make_unique<std::vector<DataType>>();
		std::for_each(composites->cbegin(), composites->cend(), [&c](const auto& i) { c->push_back(i->Clone()); });
		return std::make_unique<MatrixComposite>(Base::name, std::move(c));
	};
	virtual size_t Size() const { return composites->size(); };
	virtual void Add(DataType c) const {  composites->push_back(std::move(c)); };
private:
	std::unique_ptr<std::vector<DataType>> composites;
	virtual std::ostream& display(std::ostream& s) const { return s<<(*this); };
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixComposite& m) 
	{ 
		s<<"Name: "<<m.Name()<<std::endl;
		std::for_each(m.composites->cbegin(), m.composites->cend(), [&s](const auto& c) { 	s<<"\n"<<*c<<"\n"; }); 
		return s;  
	}
};
