#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixComposition.hpp"
#include "../M3.hpp"
#include "../MatrixStrategy.hpp"
#include "../../Builder/Builder.hpp"
#include "../../Counter/ICounter.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"

#pragma once

class XBase
{
	inline static constexpr const char TypeIdentifier[] = "Matrix";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
protected:
	XBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB):tokenFactory{fT}, elementFactory{fE}, visitorFactory{fB} {};
//	private:
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
private:
	friend std::ostream& operator<<(std::ostream& s, const XBase& m) { return s; }
};

class Readings: public XBase
{
public:
	Readings(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB): XBase{fT,fE,fB} {};
	decltype(auto) operator()()
	{
	    using MDS2 = MatrixDescriptor<2,std::string>;
	    using MDE1 = MatrixDescriptor<1,std::shared_ptr<IElement>>;
	    using MS2 = Matrix<2,MDS2>;
	    using ME1 = Matrix<2,MDE1>;
	    using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		using EVF = Factory<BaseVisitor>;

		auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));

		auto fmt=std::make_shared<Factory<IElement>>();
        auto reg = Registration<Factory<IElement>,Quantity<Energy, KiloHour>, Date, Name, Year, Index<int>, Entry, Empty>(&(*fmt));
        
		auto fbv = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,DifferenceVisitor<Quantity<Energy, KiloHour>>,DifferenceVisitor<Date>, AccumulationVisitor<Quantity<Volume>>>(&(*fbv));
        auto cv = (*fbv)("Difference","100");

        auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
        pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
        auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));

        auto tf = TypeFactory<Factory<IElement>, Quantity<Energy, KiloHour>, IBAN, Date, BIC, ID<std::string>, Name, Index<int>, Empty>();
        auto tfc = std::make_shared<TF>(fmt);
		Builder<ICounter,Counter,TopHotDesc, TopColdDesc, MiddleHotDesc, MiddleColdDesc, BottomHotDesc, BottomColdDesc> b;
		auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
					std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
					els.push_back(consV());	
					std::cout<<*i<<std::endl;  
				});

		auto med1 = Init(els);
		auto readings = med1();

        auto accBV = (*fbv)("Accumulation","100");
		accBV = readings.Accept(std::move(accBV));
		auto accV = accBV->template As<AccumulationVisitor<Quantity<Volume>>>();
		auto d3 = std::make_shared<Quantity<Volume>>((*accV(0,2)).To<Quantity<Volume>>());
		auto d2 = std::make_shared<Quantity<Volume>>((*accV(2,4)).To<Quantity<Volume>>());
		auto d1 = std::make_shared<Quantity<Volume>>((*accV(4,6)).To<Quantity<Volume>>());

		std::vector<std::shared_ptr<IElement>> elements = { d1, d2 ,d3 };
		auto m = Init(elements);
        return m();
	}
};
