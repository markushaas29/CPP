#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../MatrixComposition.hpp"
#include "../M3.hpp"
#include "../MatrixCategory.hpp"
#include "../MatrixQuery.hpp"
#include "../MatrixMultiCategory.hpp"
#include "../MatrixStrategy.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"

#pragma once

class Readings
{
	public:
		Readings(std::shared_ptr<Factory<IToken>> f):tokenFactory{f} {};
		decltype(auto) operator()()
		{
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MDE1 = MatrixDescriptor<1,std::shared_ptr<IElement>>;
		    using MS2 = Matrix<2,MDS2>;
		    using ME1 = Matrix<2,MDE1>;
		    using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
			using EVF = Factory<BaseVisitor>;

			auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"ValueToken"}, {"EmptyToken"}});
            elementTokens->push_back(std::make_unique<EnergyToken>());
            elementTokens->push_back(std::make_unique<VolumeToken>());
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

			auto CtrFs = std::vector<std::string>{{ "/home/markus/Downloads/CSV_TestFiles_2/THot.csv" }, { "/home/markus/Downloads/CSV_TestFiles_2/TCold.csv" },
													{ "/home/markus/Downloads/CSV_TestFiles_2/MHot.csv" }, { "/home/markus/Downloads/CSV_TestFiles_2/MCold.csv" },
													{ "/home/markus/Downloads/CSV_TestFiles_2/BHot.csv" }, { "/home/markus/Downloads/CSV_TestFiles_2/BCold.csv" }};
			auto els = std::vector<std::shared_ptr<IElement>>{};
			for(auto f : CtrFs)
			{
				auto mvr = MatrixReader(f);
				auto mv = mvr.M<2>();
				auto mctr = mv.Parse(matcher);
				std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
				civ = mctr.Accept(std::move(civ));
				auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
				els.push_back(consV());	
			}

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
	private:
		std::shared_ptr<Factory<IToken>> tokenFactory;
		std::shared_ptr<Factory<IElement>> elementFactory;
		std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
};
