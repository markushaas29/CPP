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
	virtual Q operator()(T& m) const = 0;
	virtual std::string_view Name() const = 0;
	//virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const = 0;
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
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixComposition";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MatrixComposition(std::unique_ptr<std::vector<PredicateType>> p, std::unique_ptr<std::vector<VisitorType>> v, const std::string& n): predicates{std::move(p)}, visitors{std::move(v)}, name{n} {}
	virtual Q operator()(Base::MatrixType& m) const
	{
		Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement> > > mP = m | predicates->at(0)->Clone();

		auto cv = mP.Accept(visitors->at(0)->Copy());
		std::cout<<"MV"<<mP<<std::endl;
		auto i =((cv->template As<AccumulationVisitor>())());
		std::cout<<"MV"<<i<<std::endl;
		return Q(5);
	}
	//virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const { return std::make_unique<MatrixComposition>(predicates,visitors,name);};
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

	MatrixComposite(PredicateFactory f, VisitorFactory v, const std::string& n): name{n}, predicates{f}, visitors{v}, composites{std::make_unique<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>>()} {}
	virtual Q operator()(Base::MatrixType& m) const
	{

		auto mP = m | (*predicates)("EqualVisitor", { "IBAN", "DE12660623660000005703"}) | (*predicates)("EqualVisitor", { "Year", "2023"}) | (*predicates)("EqualVisitor", { "Entry", "501000000891/Grundsteuer"});
		auto cv = (*visitors)("Accumulation","100");
		cv = mP.Accept(std::move(cv));
		std::cout<<"MV"<<mP<<
        (cv->As<AccumulationVisitor>())()<<std::endl;
		return Q(5);
	}
	//virtual std::unique_ptr<IMatrixComposite<T,Q>> Clone() const { return std::make_unique<MatrixComposite>(predicates,visitors,name);};
	virtual std::string_view Name() const { return name; };
	//virtual void Add(std::unique_ptr<IMatrixComposite<T,Q>> c) const {  composites->push_back(c->Clone()); };
private:
	std::unique_ptr<std::vector<std::unique_ptr<IMatrixComposite<T,Q>>>> composites;
	std::string name;
	PredicateFactory predicates;
	VisitorFactory visitors;
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
