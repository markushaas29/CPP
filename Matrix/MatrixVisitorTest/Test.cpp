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

			auto mpCleaning = mp3 | EqualVisitor::Make(IBAN::Make("DE05100110012620778704")) | EqualVisitor::Make(Year::Make("2022"));
            assert(mpCleaning[0].Rows()==3);
            auto mCleaning = mpCleaning.Cols(1);

			auto fmt=std::make_shared<Factory<IElement>>();
            auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty, Year>(&(*fmt));
            
            Factory<IToken> fmt2;
            auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
            auto qp2 = fmt2("SumToken","100");
            std::cout<<*qp2<<std::endl;
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
            auto tfc = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>(fmt);

            //assert((double)(mCleaning.To<Quantity<Sum>>()[0].ColSum())[0]==-214.2);
            
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetG = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2022"}, {"C","Grundsteuer"}}}; 
//            auto mDG = MatrixQuery<decltype(m22S),std::string>(pfs, {fUDetG});
//            auto mPropertyTax =m22_23.M(mDG).Cols(4,6,7,9,11);
//            assert(mPropertyTax.Rows()==4);
//            assert(Quantity<Sum>(mPropertyTax.ColSum(4))==Quantity<Sum>(-423.01));
			
//            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUW  = { "A",  {{"EQ", "DE44600501010008017284"}, {"C","2022"}}}; 
//            auto mW = MatrixQuery<decltype(m22S),std::string>(pfs, {fUW});
//            auto mWasteFees =m22_23.M(mW).Cols(4,6,7,9,11);
//            assert(mWasteFees.Rows()==2);
//            assert(Quantity<Sum>(mWasteFees.ColSum(4))==Quantity<Sum>(-322.0));
            
			auto mpInsurance = mp3 | tfc("EqualVisitor", { "IBAN", "DE97500500000003200029"}) | EqualVisitor::Make(Year::Make("2022"));
            std::cout<<"MatrixQuery a:\n"<<mpCleaning<<std::endl;
            assert(mpInsurance[0].Rows()==1);
			auto mpInsurance2022 = mp3[0] | tfc("EqualVisitor", { "IBAN", "DE97500500000003200029"}) | EqualVisitor::Make(Year::Make("2022"));
            assert(mpInsurance2022.Rows()==1);
			std::cout<<"M3 D2 RES:"<<mpInsurance2022.Cols(1).To<Quantity<Sum>>().ColSum()<<std::endl;
            assert((mpInsurance2022.Cols(1).To<Quantity<Sum>>().ColSum())[0]==Quantity<Sum>(-1671.31));
			
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUZie  = { "A",  {{"EQ", "DE10660501011022126625"}, {"C", "Miete"},{"C","2022"}}}; 
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUZei  = { "A",  {{"C", "Zeiher"}, {"C", "Miete"},{"C","2022"}}}; 
//            auto mZ = MatrixQuery<decltype(m22S),std::string>(pfs, {fUZie, fUZei});
//            auto mZeiher =m22_23.M(mZ).Cols(4,6,7,9,11);
//            assert(mZeiher.Rows()==12);
//            assert(Quantity<Sum>(mZeiher.ColSum(4))==Quantity<Sum>(9000));
			
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUBru  = { "A",  {{"EQ", "DE83660623660009262008"}, {"C", "MIETE"},{"C","2022"}}}; 
//            auto mB = MatrixQuery<decltype(m22S),std::string>(pfs, {fUBru});
//            auto mBrustat =m22_23.M(mB).Cols(4,6,7,9,11);
//            assert(mBrustat.Rows()==12);
//            assert(Quantity<Sum>(mBrustat.ColSum(4))==Quantity<Sum>(7720));
			
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUProp = { "A",  {{"EQ", "DE05100110012620778704"}, {"C","2023"}}}; 
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUJansen = { "A",  {{"EQ", "DE08548500101700257437"}, {"C","2023"}}}; 
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fURast = { "A",  {{"EQ", "DE79660623660000101303"}, {"C","2023"}}}; 
//            auto mBC23 = MatrixQuery<decltype(m22S),std::string>(pfs, {fUProp, fUJansen, fURast});
//            
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUEnBW = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","2022"}, {"C","Strom Abschlagsforderung"}, {"C","701006843905"}}}; 
//            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUErdgas = { "A",  {{"EQ","DE68600501010002057075"}, {"C","2022"}, {"C","Gas Abschlagsforderung"}, {"C","8201090081"}}};
//            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUErdgasInv = { "A",  {{"EQ","DE68600501010002057075"}, {"C","2023"}, {"C","Rechnung"}, {"C","8201090081"}}};
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUEnBWInv = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","2023"}, {"C","Rechnung"}, {"C","701006843905"} }}; 
//			auto mq39 = MatrixQuery<decltype(m22S),std::string>(pfs, {fUEnBW, fUErdgas, fUEnBWInv, fUErdgasInv});
//            auto Heating =m22_23.M(mq39).Cols(4,6,7,9,11);
//			auto mcP = Heating.Cols(0,2,4).Parse(matcher);
//			assert(Heating.Rows()==25);
//			assert(Quantity<Sum>(Heating.ColSum(4))==Quantity<Sum>(-2048.23));
//
//			Year y{2022};
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_EnBW = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","Strom Abschlagsforderung"}, {"C","701006843905"}}}; 
//            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_Erdgas = { "A",  {{"EQ","DE68600501010002057075"},  {"C","Gas Abschlagsforderung"}}};
//            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_ErdgasInv = { "A",  {{"EQ","DE68600501010002057075"},  {"C","Rechnung"}, {"C","8201090081"}}};
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_EnBWInv = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","Rechnung"}, {"C","701006843905"} }}; 
//			YearStrategy<decltype(m22S)> un( pfs,{fU_EnBW, fU_Erdgas, fU_EnBWInv, fU_ErdgasInv}, y, "BuildingInsurance");
//			assert(un.Name()=="BuildingInsurance");
//			auto qms = un(m22S);
			
//			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_DetG = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","Grundsteuer"}}}; 
//			YearStrategy<decltype(m22S)> ys( pfs,{fU_DetG}, y,"BuildingInsurance");
//			std::cout<<"ID "<<ys(m22S)<<std::endl;
//            assert(ys(m22S).Result()==Quantity<Sum>(-423.01));
//            assert(m22S.M(ys).Result()==Quantity<Sum>(-423.01));
//
//			auto id = std::string("8201090081");
//			IDStrategy<decltype(m22S)> ids(pfs,{fU_Erdgas}, y, id,"Gas");
//            assert(ids(m22S).Result()==Quantity<Sum>(-2113.00));
//            
//			auto msR = m22_23.M(ids);
//			auto msR2 = msR.Compose(msR);
//            assert(ys(m22S).Result()==Quantity<Sum>(-423.01));
//			
//			BaseMatrixStrategy<decltype(m22S)> ws( pfs,{fU_DetG}, "With");
//			auto mb = ws.With(y);
//            assert(m22S.M(mb).Result()==Quantity<Sum>(-423.01));
//		
//			std::vector<std::shared_ptr<IPredicateVisitor>> vip1;                                            
//	        vip1.push_back(std::make_shared<EqualVisitor>(std::make_unique<Month>(1)));
//			res3 = mp3.Accept(vip1);
//
			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
