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

class MatrixVisitorTest2023
{
	public:
		int Run()
		{
			std::cout<<"START MatrixVisitor 2023"<<std::endl;
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MS2 = Matrix<2,MDS2>;
		    using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
			using EVF = Factory<BaseVisitor>;
			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN.csv" };
			auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
            auto mrR = MatrixReader(rpath);
            auto mR = mrR.M<2>().Cols(3,4,5,6,7,8);
            auto msm = mR.To<Quantity<Scalar>>();
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);
			auto mS = MatrixReader(sNew).M<2>();
			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();
			auto t = false;
			std::vector<MS2> m22_23v{m22S, m23S};
			M3 m22_23(m22_23v);
 			

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
			
            Matcher imatcher(std::move(vi));
			
			auto vsi = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
            vsi->push_back(std::make_unique<NameIndexToken>());
            vsi->push_back(std::make_unique<StageIndexToken>());
            vsi->push_back(std::make_unique<WasteIndexToken>());
            vsi->push_back(std::make_unique<HeatingIndexToken>());
            vsi->push_back(std::make_unique<CleaningIndexToken>());
            vsi->push_back(std::make_unique<SewageIndexToken>());
            vsi->push_back(std::make_unique<PropertyTaxIndexToken>());
            vsi->push_back(std::make_unique<InsuranceIndexToken>());
            Matcher smatcher(std::move(vsi));

			auto mp3 = m22_23.Match(imatcher).Parse(matcher);
   //         std::cout<<mp3<<std::endl;
			
			auto fmt=std::make_shared<Factory<IElement>>();
            auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Index, Entry,Empty>(&(*fmt));
            
            Factory<IToken> fmt2;
            auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
            auto qp2 = fmt2("SumToken","100");
            
			auto fbv = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>>(&(*fbv));
            auto cv = (*fbv)("Accumulation","100");
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
            auto tfc = std::make_shared<TF>(fmt);

			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> properUnits{{"EqualVisitor", { "IBAN", "DE05100110012620778704"}}, {"EqualVisitor", { "Year", "2023"}}};
			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> jansenUnits{{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Year", "2023"}}};
			std::vector<FactoryUnit<std::string, std::string>> fv{{"Accumulation","100"}};
			auto properUnitss = (*tfc)(properUnits);
			auto mv = MatrixComposition<decltype(mp3)>((*tfc)(properUnits),(*fbv)(fv),"Cleaning");
			auto mcj = MatrixComposition<decltype(mp3)>((*tfc)(jansenUnits),(*fbv)(fv),"Cleaning");
			//std::cout<<"Cleanig:"<<mcj(mp3)<<std::endl;
			auto mcc = MatrixComposition<decltype(mp3)>::Create(tfc,fbv,"Cleaning",jansenUnits,fv);
			auto clP = (*mcc)(mp3);
			std::cout<<"Cleanig:"<<(*((*mcc)(mp3)))<<std::endl;
			
			std::vector<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>> cleanings = { properUnits, jansenUnits };
			auto mccs = MatrixComposite<decltype(mp3)>::Create(tfc,fbv,"Cleaning", cleanings,fv);

			auto mc = MatrixComposite<decltype(mp3)>("Compsite");
			mv.Clone();
			mc.Add(mv.Clone());
            assert(mc.Size()==1);
			mc.Add(mv.Clone());
            assert(mc.Size()==2);
			auto mc2 = mc.Clone();
            assert(mc2->Size()==2);
			auto mc3 = MatrixComposite<decltype(mp3)>("Compsite", mv.Clone());
            assert(mc3.Size()==1);
			auto cleaningR = (*mccs)(mp3);
			std::cout<<"Comp:"<<cleaningR->Value()<<std::endl;
            assert(cleaningR->Value()==Quantity<Sum>(-662.74));
////			cv = mpCleaning.Accept(std::move(cv));
////		  	(cv->As<AccumulationVisitor>())();
////          	assert((cv->As<AccumulationVisitor>())()==Quantity<Sum>(-214.20));
//
////			auto mPropertyTax = mp3 | (*tfc)("EqualVisitor", { "IBAN", "DE12660623660000005703"})| (*tfc)("EqualVisitor", { "Year", "2023"}) | (*tfc)("EqualVisitor", { "Entry", "501000000891/Grundsteuer"}) ;
////          	assert(mPropertyTax.Rows()==4);
////          	assert((mPropertyTax.Cols(4).To<Quantity<Sum>>().ColSum()[0])==Quantity<Sum>(-423.01));
////			cv = mPropertyTax.Accept(std::move(cv));
////		  	(cv->As<AccumulationVisitor>())();
////			std::cout<<"Property Tax:"<<mPropertyTax<<std::endl;
////          	assert((cv->As<AccumulationVisitor>())()==Quantity<Sum>(-423.01));
////		
////			auto mWasteFees = mp3 | (*tfc)("EqualVisitor", { "IBAN", "DE44600501010008017284"})| (*tfc)("EqualVisitor", { "Year", "2023"});
////            std::cout<<mWasteFees<<std::endl;
////            assert(mWasteFees.Rows()==2);
////			std::cout<<"Waste Fees:"<<mWasteFees<<std::endl;
////          	//assert((mWasteFees.Cols(4).To<Quantity<Sum>>().ColSum()[0])==Quantity<Sum>(-322));
////            
////			auto mpInsurance = mp3 | (*tfc)("EqualVisitor", { "IBAN", "DE97500500000003200029"}) | (*tfc)("EqualVisitor", { "Year", "2023"});
////            //assert(mpInsurance.Rows()==1);
////			std::cout<<"Insurance:"<<mpInsurance<<std::endl;
////            //assert((mpInsurance2023.Cols(4).To<Quantity<Sum>>().ColSum())[0]==Quantity<Sum>(-1671.31));
////			
////			auto mZie = mp3 | (*tfc)("EqualVisitor", { "IBAN", "DE10660501011022126625"})| (*tfc)("EqualVisitor", { "Year", "2023"});
////			auto mZei = mp3 | (*tfc)("EqualVisitor", { "IBAN", "DE47660501011020531958"})| (*tfc)("EqualVisitor", { "Year", "2023"});
////            //assert((mZei.Rows()+mZie.Rows())==13);
////			std::cout<<"Zei:"<<mZei<<std::endl;
//////            assert(Quantity<Sum>(mZeiher.ColSum(4))==Quantity<Sum>(9000));
////			
////			auto mBrustat = mp3 | (*tfc)("EqualVisitor", { "Entry", "MIETE"})| (*tfc)("EqualVisitor", { "IBAN", "DE83660623660009262008"})| (*tfc)("EqualVisitor", { "Year", "2023"});
////            //assert(mBrustat.Rows()==12);
////          	//assert((mBrustat.Cols(4).To<Quantity<Sum>>().ColSum()[0])==Quantity<Sum>(7720));
////			std::cout<<"Brus:"<<mBrustat<<std::endl;
////			
//////			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> properUnitsProp = { "A",  {{"EQ", "DE05100110012620778704"}, {"C","2023"}}}; 
//////			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> jansenUnitsansen = { "A",  {{"EQ", "DE08548500101700257437"}, {"C","2023"}}}; 
//////			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> properUnitsRast = { "A",  {{"EQ", "DE79660623660000101303"}, {"C","2023"}}}; 
//////            auto mBC23 = MatrixQuery<decltype(m22S),std::string>(pfs, {properUnitsProp, jansenUnitsansen, properUnitsRast});
//////            
////			//auto mEnBW = mp3 | (*tfc)("EqualVisitor", { "Entry", "701006843905"})| (*tfc)("EqualVisitor", { "Entry", "Abschlagsforderung"}) | (*tfc)("EqualVisitor", { "IBAN", "DE56600501017402051588"})| (*tfc)("EqualVisitor", { "Year", "2023"});
			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> properUnitsEnBW{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}}};
			auto me = MatrixComposition<decltype(mp3)>((*tfc)(properUnitsEnBW),(*fbv)(fv),"EnBW");

			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> properUnitsGas{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2023"}}};
			auto mg = MatrixComposition<decltype(mp3)>((*tfc)(properUnitsGas),(*fbv)(fv),"EnBW");

			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> properUnitsGasI{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2024"}}};
			auto mgi = MatrixComposition<decltype(mp3)>((*tfc)(properUnitsGasI),(*fbv)(fv),"EnBW");
		
			std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> properUnitsEnBWI{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2024"}}};
			auto mei = MatrixComposition<decltype(mp3)>((*tfc)(properUnitsEnBWI),(*fbv)(fv),"EnBW");
            
			auto mcHeating = MatrixComposite<decltype(mp3)>("Heating", me.Clone());
			mcHeating.Add(mg.Clone());
			mcHeating.Add(mgi.Clone());
			mcHeating.Add(mei.Clone());
            assert(mcHeating.Size()==4);
			auto heatingR = mcHeating(mp3);
			//assert(heatingR.Value()==Quantity<Sum>(-3558.17));
  			std::cout<<"Heating:"<<*heatingR<<std::endl;

			std::vector<std::vector<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>> allFactoryUnits = 
			{
				{
					{
						{"EqualVisitor", { "IBAN", "DE44600501010008017284"}}, {"EqualVisitor", { "Year", "2023"}} // Waste
					}
				}, 
				{
					{
						{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}} //Heatung
					},
					{
						{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2023"}}
					}
				},
				{
					{
						{"EqualVisitor", { "IBAN", "DE97500500000003200029"}}, {"EqualVisitor", { "Year", "2023"}} // Insurance
					}
				},
				{
					{ //Cleaning
						properUnits, 
						jansenUnits
					}
				},
				{
					{
						{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "501000000891/Grundsteuer"}} //Grundsteuer
					}
				},
				{
					{
						{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "Abschlag/Abwasser"}}, //Abwasser
					},
					{
						{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2024"}}, {"EqualVisitor", { "Entry", "Rechung/Abwasser"}} //Abwasser
					}
				}
			};

			auto all = std::make_unique<MatrixComposite<decltype(mp3)>>("All");//, mcHeating.Clone());

			for(uint i = 0; i < allFactoryUnits.size(); ++i)
					all->Add(MatrixComposite<decltype(mp3)>::Create(tfc,fbv,"", allFactoryUnits[i],fv));
			//std::unique_ptr<IMatrixQueryResult<Quantity<Sum>,Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>> result = (*all)(mp3);
			auto result = (*all)(mp3);
			std::cout<<"\n-------------------All---------------------\n:\n"<<(*(*all)(mp3))<<std::endl;
			std::cout<<"\n-------------------All Res---------------------\n:\n"<<result->Elements()<<std::endl;
			auto ms = result->Elements().To<Quantity<Sum>>();

			auto cs = msm / mR.ColSum();
			std::cout<<"\n-------------------Summen:---------------------\n:\n"<<ms<<"\nDivider"<<cs<<std::endl;
			auto mcs = cs * ms;
			std::cout<<"\n-------------------All Calc---------------------\n:\n"<<mcs<<std::endl;
			auto mpsA = mS.Match(smatcher).Parse(matcher);
			auto mps = mpsA.Cols(2,3,4,5,6,7);
			auto mpsM = (msm / msm.ColSum());
			auto res = mpsM * ms;
			std::cout<<"\n-------------------MPS---------------------\n:\n"<<mps<<std::endl;
			std::cout<<"\n-------------------MPS Mul---------------------\n:\n"<<mpsM<<std::endl;
			std::cout<<"\n-------------------MPS Mul---------------------\n:\n"<<res<<std::endl;
			//assert(mcs[0].To<Quantity<Sum>>()==Quantity<Sum>{-1346.31});
			//v = ms[5];
			//assert(allR==)
            //assert(mEnBWI.Rows()==2);
//			auto mcP = Heating.Cols(0,2,4).Parse(matcher);
//			assert(Heating.Rows()==25);
//			assert(Quantity<Sum>(Heating.ColSum(4))==Quantity<Sum>(-2048.23));
//
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
