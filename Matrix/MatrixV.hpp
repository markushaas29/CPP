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
class IMatrixV
{
public:
	using MatrixType = T;
	using QuantityType = Q;
	//using UnitType = U;
	using ElementType = T::ElementType;
	//using ResultType = StrategyResult<QuantityType,MatrixType,UnitType>;
	virtual Q operator()(T& m) const = 0;
	virtual std::string_view Name() const = 0;
//	virtual const std::vector<UnitType> Units() const = 0;
};

template<typename T, typename F, typename Q = Quantity<Sum>>
class BaseMatrixV : public IMatrixV<T,Q>//,Q,FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>>
{
protected:
	using Base = IMatrixV<T,Q>;
	using QueryType = MatrixQuery<T>;
	using UnitType = FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>>;
	using PredicateFactory = std::shared_ptr<F>;
	using VisitorFactory = std::shared_ptr<Factory<BaseVisitor>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixV";
    inline static constexpr Literal TypeId{TypeIdentifier};

	BaseMatrixV(PredicateFactory f, VisitorFactory v, const std::string& n): name{n}, predicates{f}, visitors{v} {}
	virtual Q operator()(Base::MatrixType& m) const
	{

		auto mP = m | (*predicates)("EqualVisitor", { "IBAN", "DE12660623660000005703"}) | (*predicates)("EqualVisitor", { "Year", "2023"}) | (*predicates)("EqualVisitor", { "Entry", "501000000891/Grundsteuer"});
		auto cv = (*visitors)("Accumulation","100");
		cv = mP.Accept(std::move(cv));
		std::cout<<"MV"<<mP<<
        (cv->As<AccumulationVisitor>())()<<std::endl;
		return Q(5);
	}
	virtual std::string_view Name() const { return name; };
protected:
	virtual std::vector<UnitType> enrich(const std::vector<UnitType>& v) const { return v;};// = 0;
private:
	std::string name;
	PredicateFactory predicates;
	VisitorFactory visitors;
	template<typename U> using IsT =  Is<U,TypeId>;
	friend std::ostream& operator<<(std::ostream& s, const BaseMatrixV& m) 
	{ 
		s<<"Name: "<<m.name<<std::endl;
		std::vector<UnitType> eunits = m.enrich(m.units);
		std::for_each(eunits.begin(), eunits.end(), [&](auto& u) { s<<u<<"\n"; });
		return s;  
	}
};
