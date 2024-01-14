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
 			
            Factory<IToken> fmt2;
            auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken>(&fmt2);
            auto v = fmt2({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
            Matcher matcher(std::move(v));

            Factory<IToken> fIIT;
            auto rIIT = Registration<Factory<IToken>, IBANIndexToken, BICIndexToken, NameIndexToken, SumIndexToken, UseIndexToken, DateIndexToken>(&fIIT);
            auto vi = fIIT({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
            Matcher imatcher(std::move(vi));
			
            Factory<IToken> fSIT;
            auto rISIT = Registration<Factory<IToken>, NameIndexToken, StageIndexToken, WasteIndexToken, HeatingIndexToken, CleaningIndexToken, SewageIndexToken, PropertyTaxIndexToken, InsuranceIndexToken>(&fSIT);
            auto vsi = fSIT({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"}, });
            Matcher smatcher(std::move(vsi));

			auto mp3 = m22_23.Match(imatcher).Parse(matcher);
   //         std::cout<<mp3<<std::endl;
			
            auto fmt = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Index<int>, Entry,Empty>();

			auto fbv = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>>(&(*fbv));
            auto cv = (*fbv)("Accumulation","100");
 
            auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
            pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));
 
            auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index<int>, Empty>();
            auto tfc = std::make_shared<TF>(fmt);

		 	std::vector<FactoryUnit<std::string, std::string>> fv{{"Accumulation","100"}};
			auto mc = MatrixComposite<decltype(mp3)>("Compsite");
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
					},
					{
						{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2024"}} //Heatung
					},
					{
						{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2024"}}
					}
				},
				{
					{
						{"EqualVisitor", { "IBAN", "DE97500500000003200029"}}, {"EqualVisitor", { "Year", "2023"}} // Insurance
					}
				},
				{
					{ //Cleaning
						{"EqualVisitor", { "IBAN", "DE05100110012620778704"}}, {"EqualVisitor", { "Year", "2023"}}
					},
					{ //Cleaning
						{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Year", "2023"}}
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
			auto result = (*all)(mp3);
			std::cout<<"\n-------------------All---------------------\n:\n"<<(*(*all)(mp3))<<std::endl;
			auto ms = result->Elements().To<Quantity<Sum>>();

			auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).To<Quantity<Scalar>>();
			mps = mps.Set(Quantity<Scalar>(47.361),1,5);
			mps = mps.Set(Quantity<Scalar>(62.424),2,5);
			mps = mps.Set(Quantity<Scalar>(33.559),3,5);
			auto mpsM = (mps / mps.ColSum());
			auto res = mpsM * ms;
			std::cout<<"\n-------------------MPS---------------------\n:\n"<<mps<<std::endl;
			std::cout<<"\n-------------------MPS divided by ColSum---------------------\n:\n"<<mpsM<<std::endl;
			std::cout<<"\n-------------------MPS Result---------------------\n:\n"<<res<<std::endl;
			//assert(res[1].To<Quantity<Sum>>()==Quantity<Sum>{-2487.87});
			//assert(res[2].To<Quantity<Sum>>()==Quantity<Sum>{-2627.11});
			//assert(res[3].To<Quantity<Sum>>()==Quantity<Sum>{-2642.64});
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
