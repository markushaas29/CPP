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
#include "../MatrixAnalyzer.hpp"
#include "../M3.hpp"
#include "../MatrixCategory.hpp"
#include "../MatrixVisitor.hpp"
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

class MatrixVisitorTest
{
	public:
		int Run()
		{
			std::cout<<"START MatrixVisitor"<<std::endl;
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MS2 = Matrix<2,MDS2>;
		
			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);
			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();
			auto t = false;
			std::vector<MS2> m22_23v{m22S, m23S};
			M3 m22_23(m22_23v);
 			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetAb = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2023"}, {"C","Rechnung/Abwasser"}}}; 
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetAb2 = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2022"}, {"C","Abschlag/Abwasser"}}}; 

			auto v = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
            v->push_back(std::make_unique<DateToken>());
            v->push_back(std::make_unique<IBANToken>("DE19660623660009232702"));
            v->push_back(std::make_unique<BICToken>());
            v->push_back(std::make_unique<WordToken>());
            v->push_back(std::make_unique<SumToken>());
            v->push_back(std::make_unique<ValueToken>());
            v->push_back(std::make_unique<QuantityToken>());
            v->push_back(std::make_unique<KeyValueToken>());
            v->push_back(std::make_unique<EmptyToken>());
            v->push_back(std::make_unique<IBANIndexToken>());

            Matcher matcher(std::move(v));

			auto vi = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
            vi->push_back(std::make_unique<NameIndexToken>());
            vi->push_back(std::make_unique<DateIndexToken>());
            vi->push_back(std::make_unique<IBANIndexToken>());
            vi->push_back(std::make_unique<BICIndexToken>());
            vi->push_back(std::make_unique<SumIndexToken>());
            vi->push_back(std::make_unique<UseIndexToken>());
			
			std::vector<std::shared_ptr<IPredicateVisitor>> vip;                                            
	        vip.push_back(std::make_shared<LessVisitor>(std::make_unique<Quantity<Sum>>(-40))); 
	        vip.push_back(std::make_shared<EqualVisitor>(std::make_unique<IBAN>("DE56600501017402051588")));
	        vip.push_back(std::make_shared<EqualVisitor>(std::make_unique<Year>(2022)));

            Matcher imatcher(std::move(vi));

			auto mp3 = m22_23.Match(imatcher).Parse(matcher);
			auto res3 = mp3.Accept(vip);
			
			auto fmt=std::make_shared<Factory<IElement>>();
            auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Index, Entry,Empty>(&(*fmt));
            
            Factory<IToken> fmt2;
            auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
            auto qp2 = fmt2("SumToken","100");
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
            auto tfc = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>(fmt);

			auto vc = tfc({{"EqualVisitor", { "IBAN", "DE05100110012620778704"}},{"EqualVisitor", { "Year", "2022"}}});
			for(auto& v : vc)
				v->Display(std::cout);
			auto mpCleaning = mp3 | tfc("EqualVisitor", { "IBAN", "DE05100110012620778704"}) | tfc("EqualVisitor", { "Year", "2022"});
            std::cout<<mpCleaning<<std::endl;
            assert(mpCleaning.Rows()==3);
            auto mCleaning = mpCleaning.Cols(1);

            //assert((double)(mCleaning.To<Quantity<Sum>>()[0].ColSum())[0]==-214.2);
            
            //std::cout<<mp3<<std::endl;
			auto mPropertyTax = mp3 | tfc("EqualVisitor", { "IBAN", "DE12660623660000005703"})| tfc("EqualVisitor", { "Year", "2022"}) | tfc("EqualVisitor", { "Entry", "501000000891/Grundsteuer"}) ;
            std::cout<<"Grundsteuer:\n"<<mPropertyTax<<std::endl;
          	assert(mPropertyTax.Rows()==4);
          	assert((mPropertyTax.Cols(3).To<Quantity<Sum>>().ColSum()[0])==Quantity<Sum>(-423.01));
		
			auto mWasteFees = mp3 | tfc("EqualVisitor", { "IBAN", "DE44600501010008017284"})| tfc("EqualVisitor", { "Year", "2022"});
            assert(mWasteFees.Rows()==2);
            //assert(Quantity<Sum>(mWasteFees.ColSum(1))==Quantity<Sum>(-322.0));
            
			auto mpInsurance = mp3 | tfc("EqualVisitor", { "IBAN", "DE97500500000003200029"}) | tfc("EqualVisitor", { "Year", "2022"});
            assert(mpInsurance.Rows()==1);
			auto mpInsurance2022 = mp3[0] | tfc("EqualVisitor", { "IBAN", "DE97500500000003200029"}) | tfc("EqualVisitor", { "Year", "2022"});
            assert(mpInsurance2022.Rows()==1);
			std::cout<<"M3 D2 RES:"<<mpInsurance2022.Cols(1).To<Quantity<Sum>>().ColSum()<<std::endl;
            assert((mpInsurance2022.Cols(3).To<Quantity<Sum>>().ColSum())[0]==Quantity<Sum>(-1671.31));
			tfc("EqualVisitor", { "Year", "2022"});
			
			auto mZie = mp3 | tfc("EqualVisitor", { "IBAN", "DE10660501011022126625"})| tfc("EqualVisitor", { "Year", "2022"});
			auto mZei = mp3 | tfc("EqualVisitor", { "IBAN", "DE47660501011020531958"})| tfc("EqualVisitor", { "Year", "2022"});
            assert((mZei.Rows()+mZie.Rows())==13);
//            assert(Quantity<Sum>(mZeiher.ColSum(4))==Quantity<Sum>(9000));
			
			auto mBrustat = mp3 | tfc("EqualVisitor", { "Entry", "MIETE"})| tfc("EqualVisitor", { "IBAN", "DE83660623660009262008"})| tfc("EqualVisitor", { "Year", "2022"});
            std::cout<<mBrustat<<std::endl;
            assert(mBrustat.Rows()==12);
            //assert(Quantity<Sum>(mBrustat.ColSum(4))==Quantity<Sum>(7720));
			
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUProp = { "A",  {{"EQ", "DE05100110012620778704"}, {"C","2023"}}}; 
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUJansen = { "A",  {{"EQ", "DE08548500101700257437"}, {"C","2023"}}}; 
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fURast = { "A",  {{"EQ", "DE79660623660000101303"}, {"C","2023"}}}; 
//            auto mBC23 = MatrixQuery<decltype(m22S),std::string>(pfs, {fUProp, fUJansen, fURast});
//            
			//auto mEnBW = mp3 | tfc("EqualVisitor", { "Entry", "701006843905"})| tfc("EqualVisitor", { "Entry", "Abschlagsforderung"}) | tfc("EqualVisitor", { "IBAN", "DE56600501017402051588"})| tfc("EqualVisitor", { "Year", "2022"});
			auto mEnBW = mp3 | tfc("EqualVisitor", { "Entry", "Abschlagsforderung"}) | tfc("EqualVisitor", { "IBAN", "DE56600501017402051588"})| tfc("EqualVisitor", { "Year", "2022"});
            std::cout<<mEnBW<<std::endl;
			auto mGas = mp3 | tfc("EqualVisitor", { "Entry", "Abschlagsforderung"}) | tfc("EqualVisitor", { "IBAN", "DE68600501010002057075"})| tfc("EqualVisitor", { "Year", "2022"});
            assert(mGas.Rows()==11);
			auto mGasI = mp3 | tfc("EqualVisitor", { "Entry", "Rechnung"}) | tfc("EqualVisitor", { "IBAN", "DE68600501010002057075"})| tfc("EqualVisitor", { "Year", "2023"});
            assert(mGasI.Rows()==1);
            std::cout<<mGasI<<std::endl;
			auto mEnBWI = mp3 | tfc("EqualVisitor", { "Entry", "Rechnung"}) | tfc("EqualVisitor", { "IBAN", "DE56600501017402051588"})| tfc("EqualVisitor", { "Year", "2023"});
            std::cout<<mEnBWI<<std::endl;
            assert(mEnBWI.Rows()==2);
//			auto mcP = Heating.Cols(0,2,4).Parse(matcher);
//			assert(Heating.Rows()==25);
//			assert(Quantity<Sum>(Heating.ColSum(4))==Quantity<Sum>(-2048.23));
//
			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
