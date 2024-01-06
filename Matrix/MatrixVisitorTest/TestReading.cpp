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
		    using MS2 = Matrix<2,MDS2>;
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
            auto reg = Registration<Factory<IElement>,Quantity<Energy>, Date, Name, Year, Index, Entry, Empty>(&(*fmt));
            
            Factory<IToken> fmt2;
            //auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
            
			auto fbv = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,DifferenceVisitor<Quantity<Energy>>,DifferenceVisitor<Date>>(&(*fbv));
            auto cv = (*fbv)("Difference","100");
            //auto dtv = (*fbv)("Date","100");
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Energy>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
            auto tfc = std::make_shared<TF>(fmt);

			std::unique_ptr<BaseVisitor> dtv = std::make_unique<DifferenceVisitor<Date>>();
			cv = mp.Accept(std::move(cv));
			dtv = mp.Accept(std::move(dtv));
			auto dv = cv->template As<DifferenceVisitor<Quantity<Energy>>>();
			auto dttv = dtv->template As<DifferenceVisitor<Date>>();
            std::cout<<dv()<<std::endl;
			//assert(dv()==Quantity<Energy>(0.1));
            std::cout<<dv<<std::endl;
            std::cout<<dttv()<<std::endl;
            std::cout<<Quantity<Volume>(1.5)<<std::endl;

			std::cout<<"END Reading 2023"<<std::endl;
		   
			return 0;
		}
};
