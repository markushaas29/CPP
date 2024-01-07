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

class MatrixReadingVisitorTest
{
	public:
		int Run()
		{
			std::cout<<"START MatrixVisitor Reading 2023"<<std::endl;
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MDE1 = MatrixDescriptor<1,std::shared_ptr<IElement>>;
		    using MS2 = Matrix<2,MDS2>;
		    using ME1 = Matrix<2,MDE1>;
		    using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
			using EVF = Factory<BaseVisitor>;
			auto ur = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/Energ.csv" };
			auto mr = MatrixReader(ur);
			auto me = mr.M<2>();
			auto t = false;
			//std::vector<MS2> m22_23v{m22S, m23S};
			//M3 m22_23(m22_23v);
 			

			auto v = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
            v->push_back(std::make_unique<DateToken>());
            v->push_back(std::make_unique<WordToken>());
            v->push_back(std::make_unique<SumToken>());
            v->push_back(std::make_unique<ValueToken>());
            v->push_back(std::make_unique<EnergyToken>());
            v->push_back(std::make_unique<VolumeToken>());
            v->push_back(std::make_unique<EmptyToken>());

            Matcher matcher(std::move(v));

			auto vi = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
            vi->push_back(std::make_unique<DateIndexToken>());
            vi->push_back(std::make_unique<IBANIndexToken>());
            vi->push_back(std::make_unique<BICIndexToken>());
            vi->push_back(std::make_unique<BICIndexToken>());
			
            Matcher imatcher(std::move(vi));
	//		auto mp3 = m22_23.Match(imatcher).Parse(matcher);
			auto mp = me.Parse(matcher);
            std::cout<<mp<<std::endl;
			
			auto fmt=std::make_shared<Factory<IElement>>();
            auto reg = Registration<Factory<IElement>,Quantity<Energy, KiloHour>, Date, Name, Year, Index, Entry, Empty>(&(*fmt));
            
            Factory<IToken> fmt2;
            //auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
            
			auto fbv = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,DifferenceVisitor<Quantity<Energy, KiloHour>>,DifferenceVisitor<Date>>(&(*fbv));
            auto cv = (*fbv)("Difference","100");
            //auto dtv = (*fbv)("Date","100");
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Energy, KiloHour>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
            auto tfc = std::make_shared<TF>(fmt);

			std::unique_ptr<BaseVisitor> dtv = std::make_unique<DifferenceVisitor<Date>>();
			cv = mp.Accept(std::move(cv));
			dtv = mp.Accept(std::move(dtv));
			auto dv = cv->template As<DifferenceVisitor<Quantity<Energy, KiloHour>>>();
			auto dttv = dtv->template As<DifferenceVisitor<Date>>();
            std::cout<<dv()<<std::endl;
			
			std::unique_ptr<BaseVisitor> conv = std::make_unique<ConsumptionVisitor<Quantity<Energy, KiloHour>>>();
			conv = mp.Accept(std::move(conv));
			auto cons = conv->template As<ConsumptionVisitor<Quantity<Energy, KiloHour>>>();
			auto s = cons();	
			auto val = Quantity<Unit<0, 1, 2, -3>>(1.7);
			//assert((double)s.PureValue()==(double)val.PureValue());
            std::cout<<"Consumption "<<cons()<<std::endl;
            std::cout<<dttv()<<std::endl;
            std::cout<<Quantity<Volume>(1.5)<<std::endl;
			
			auto CtrFs = std::vector<std::string>{{ "/home/markus/Downloads/CSV_TestFiles_2/THot.csv" }, { "/home/markus/Downloads/CSV_TestFiles_2/TCold.csv" },
													{ "/home/markus/Downloads/CSV_TestFiles_2/BHot.csv" }, { "/home/markus/Downloads/CSV_TestFiles_2/BCold.csv" }};
			auto els = std::vector<std::shared_ptr<IElement>>{};
			for(auto f : CtrFs)
			{
				auto mvr = MatrixReader(f);
				auto mv = mvr.M<2>();
				auto mctr = mv.Parse(matcher);
				std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
				civ = mctr.Accept(std::move(civ));
            	std::cout<<mctr<<std::endl;
				auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
				els.push_back(consV());	
			}
			for(auto e : els)
            	std::cout<<*e<<std::endl;

			ME1 me1(MDE1{els.size()},els);
			//std::cout<<me1<<std::endl;
			std::cout<<"END Reading 2023"<<std::endl;
		   
			return 0;
		}
};
