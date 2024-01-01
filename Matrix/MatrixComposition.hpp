#include <memory>
#include <tuple>
#include <vector>
#include "MatrixQuery.hpp"
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
	//using UnitType = U;
	using ElementType = T::ElementType;
	//using ResultType = StrategyResult<QuantityType,MatrixType,UnitType>;
	using ResultType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
	virtual Q operator()(T& m) const = 0;
	virtual std::string_view Name() const = 0;
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const = 0;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixComposite& m) { return m.display(s); }
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<typename T, typename Q = Quantity<Sum>>
class MatrixComposition: public IMatrixComposite<T,Q>
{
protected:
	using Base = IMatrixComposite<T,Q>;
	using QueryType = MatrixQuery<T>;
	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
	using PredicateType = std::unique_ptr<IPredicateVisitor>;
	using VisitorType = std::unique_ptr<BaseVisitor>;
	using ResultType = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposition";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixComposition(std::unique_ptr<std::vector<PredicateType>> p, std::unique_ptr<std::vector<VisitorType>> v, const std::string& n): predicates{std::move(p)}, visitors{std::move(v)}, name{n} {}
	virtual Q operator()(Base::MatrixType& m) const
	{
		ResultType result;
		if(predicates->size()>0)
		{
			result = m |  (predicates->at(0)->Clone());
			std::for_each(predicates->cbegin(), predicates->cend(), [&](const auto& i) { result = result | (i->Clone()); });
			auto cv =result.Accept(visitors->at(0)->Copy());
			//std::cout<<"MV"<<mP<<std::endl;
			auto i =((cv->template As<AccumulationVisitor>())());
			std::cout<<"MV abc"<<i<<"\n"<<result<<std::endl;
		}
		return Q{5};
	}
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const 
	{ 
		std::unique_ptr<std::vector<PredicateType>> p;
		std::for_each(predicates->cbegin(), predicates->cend(), [&p](const auto& i) { p->push_back(i->Clone()); });
		std::unique_ptr<std::vector<VisitorType>> v;
		std::for_each(visitors->cbegin(), visitors->cend(), [&v](const auto& i) { v->push_back(i->Copy()); });
		return std::make_unique<MatrixComposition>(std::move(p),std::move(v),name);
	}
	virtual std::string_view Name() const { return name; };
private:
	std::string name;
	std::unique_ptr<std::vector<PredicateType>> predicates;
	std::unique_ptr<std::vector<VisitorType>> visitors;
	virtual std::ostream& display(std::ostream& s) const { return s<<(*this); };
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixComposition& m) 
	{ 
		s<<"Name: "<<m.name<<std::endl;
		std::for_each(m.predicates->cbegin(), m.predicates->cend(), [&s](const auto& i) { s<<*i<<"\n"; });
		//std::for_each(m.visitors->cbegin(), m.visitors->cend(), [&s](const auto& i) { s<<*i<<"\n"; });
		return s;  
	}
};

template<typename T, typename F, typename Q = Quantity<Sum>>
class MatrixComposite: public IMatrixComposite<T,Q>
{
protected:
	using Base = IMatrixComposite<T,Q>;
	using QueryType = MatrixQuery<T>;
	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
	using PredicateFactory = std::shared_ptr<F>;
	using VisitorFactory = std::shared_ptr<Factory<BaseVisitor>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposite";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixComposite(const std::string& n): name{n}, composites{std::make_unique<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>>()} {}
	MatrixComposite(const std::string& n, std::unique_ptr<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>> c): name{n}, composites{std::move(c)} {}
	virtual Q operator()(Base::MatrixType& m) const
	{
		return Q(5);
	}
	virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const 
	{ 
		std::unique_ptr<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>> c;
		std::for_each(composites->cbegin(), composites->cend(), [&c](const auto& i) { c->push_back(i->Clone()); });
		return std::make_unique<MatrixComposite>(name, std::move(c));
	};
	virtual std::string_view Name() const { return name; };
	virtual size_t Size() const { return composites->size(); };
	//virtual void Add(std::unique_ptr<IMatrixComposite<T,Q>> c) const {  composites->push_back(c->Clone()); };
private:
	std::unique_ptr<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>> composites;
	std::string name;
	virtual std::ostream& display(std::ostream& s) const { return s<<(*this); };
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixComposite& m) 
	{ 
		s<<"Name: "<<m.name<<std::endl;
		std::for_each(m.composites->cbegin(), m.composites->cend(), [&s](const auto& c) { c->display(s); }); 

		s<<"Name END: "<<m.name<<std::endl;
		return s;  
	}
};
