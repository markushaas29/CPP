#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Readings.hpp"
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
			auto u24 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2024.csv" };
			auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN.csv" };
			auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
            auto mrR = MatrixReader(rpath);
            auto mR = mrR.M<2>().Cols(3,4,5,6,7,8);
            auto msm = mR.To<Quantity<Scalar>>();
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);
			auto m24r = MatrixReader(u24);
			auto mS = MatrixReader(sNew).M<2>();
			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();
			auto m24S = m24r.M<2>();
			auto t = false;
			std::vector<MS2> accountFiles{m22S, m23S, m24S};
			M3 accountMatrix(accountFiles);

            auto tokenFactory = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken,IBANIndexToken, BICIndexToken, NameIndexToken, SumIndexToken, UseIndexToken, DateIndexToken, StageIndexToken, WasteIndexToken, HeatingIndexToken, CleaningIndexToken, SewageIndexToken, PropertyTaxIndexToken, InsuranceIndexToken, RentIndexToken, HeatExtraCostIndexToken, ExtraCostIndexToken>();
            auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"} });
            Matcher smatcher(std::move(stageIndexTokens));
            auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
            Matcher imatcher(std::move(csvIndexTokens));
            auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
            Matcher matcher(std::move(v));

			auto parsedAccountMatrix = accountMatrix.Match(imatcher).Parse(matcher);
			
            auto elementFactory = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Month,Index<int>, Entry,Empty>();
            auto typeFactory = std::make_shared<TF>(elementFactory);

			auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>>(&(*visitorFactory));
 
            auto predicateVisitorFactory = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(elementFactory);
            predicateVisitorFactory->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
            auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*predicateVisitorFactory));
		 	
			std::vector<FactoryUnitContainer<std::vector<FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>>> allFactoryUnits = 
			{
				{"Waste",
					{
					
						{"Deduction",{{"EqualVisitor", { "IBAN", "DE44600501010008017284"}}, {"EqualVisitor", { "Year", "2023"}}}} // Waste
					}
				}, 
				{"Heating",
					{
						{"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}}}},
						{"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2023"}}}},
						{"Invoice",{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2024"}}}},
						{"Rechnung",{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2024"}}}},
					}
				},
				{"BuildingInsurance",{
						{"Invoice",{{"EqualVisitor", { "IBAN", "DE97500500000003200029"}}, {"EqualVisitor", { "Year", "2023"}}}} // Insurance
					}
				},
				{"Cleaning",{
						{"Alles Proper",{{"EqualVisitor", { "IBAN", "DE05100110012620778704"}}, {"EqualVisitor", { "Year", "2023"}}}},
						{"Jansen",{{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Year", "2023"}}}},
						{"Jansen",{{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Month", "1"}}}},
						{"Rastaetter",{{"EqualVisitor", { "IBAN", "DE79660623660000101303"}}, {"EqualVisitor", { "Year", "2023"}}}},
					}
				},
				{"PropertyTax",{
						{"Deduction",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "501000000891/Grundsteuer"}}}} //Grundsteuer
					}
				},
				{"Sewage",{
						{"Deduction",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "Abschlag/Abwasser"}}}}, //Abwasser
						{"Invoice",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2024"}}, {"EqualVisitor", { "Entry", "Rechnung/Abwasser"}}}} //Abwasser
					}
				}
			};

			auto all = std::make_unique<MatrixComposite<decltype(parsedAccountMatrix)>>("All");//, mcHeating.Clone());

			std::vector<FactoryUnit<std::string, std::string>> fv{{"Accumulation"}};
			for(uint i = 0; i < allFactoryUnits.size(); ++i)
					all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
			auto result = (*all)(parsedAccountMatrix);
			//std::cout<<"\n-------------------All---------------------\n:\n"<<(*(*all)(parsedAccountMatrix))<<std::endl;
			auto ms = result->Elements().To<Quantity<Sum>>();
			auto fms = result->Funcs();
			std::cout<<"\n-------------------All---------------------\n:\n"<<fms<<std::endl;
			std::cout<<"\n-------------------All()---------------------\n:\n"<<fms()<<std::endl;
			std::cout<<"\n-------------------All[0]()---------------------\n:\n"<<fms[0]()<<std::endl;

			auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).To<Quantity<Scalar>>();
			auto stageQ = mS.Match(smatcher).Parse(matcher);
			auto payment = stageQ.Cols(8,9,10).To<Quantity<Sum>>();
			std::vector<Quantity<Sum>> extras = {{payment[1][1].To<Quantity<Sum>>()+payment[1][2].To<Quantity<Sum>>()}, {payment[2][1].To<Quantity<Sum>>()+payment[2][2].To<Quantity<Sum>>()}}; 
			std::for_each(extras.begin(), extras.end(),[&](auto& e) { e = e * Quantity<Scalar>{12}; });
			std::cout<<"\n-------------------Stages()---------------------\n:\n"<<payment<<extras[0]<<std::endl;

			const std::string path = "/home/markus/Downloads/CSV_TestFiles_2"; 
			auto readingsT = Readings<Top>{tokenFactory,elementFactory,visitorFactory, path};
			auto readingsM = Readings<Middle>{tokenFactory,elementFactory,visitorFactory, path};
			auto readingsB = Readings<Bottom>{tokenFactory,elementFactory,visitorFactory, path};
			auto stages = Stages{tokenFactory,elementFactory,visitorFactory, path};
			mps = readingsT(std::move(mps));
			mps = stages(std::move(mps));

			auto mpsM = (mps / mps.ColSum());
			auto res = mpsM * ms;
			std::cout<<"\n-------------------Fractions()---------------------\n:\n"<<fms().To<Quantity<Sum>>()<<std::endl;
			std::cout<<"\n-------------------Costs---------------------\n:\n"<<ms<<std::endl;
			std::cout<<"\n-------------------Stages divided by ColSum---------------------\n:\n"<<mpsM<<std::endl;
			std::cout<<"\n-------------------Stages / Colsum() + Costs---------------------\n:\n"<<res<<std::endl;
			std::cout<<"\n-------------------MPS Result()---------------------\n:\n"<<res()<<std::endl;
			std::cout<<"\n-------------------MPS Bru---------------------\n:\n"<<res[1]<<std::endl;
			std::cout<<"\n-------------------MPS Bru Result() =---------------------\n:\n"<<res[1]()<<std::endl;
			std::cout<<"\n-------------------MPS Zei---------------------\n:\n"<<res[2]<<std::endl;
			std::cout<<"\n-------------------MPS Zei Result() =---------------------\n:\n"<<res[2]()<<std::endl;
			auto resQ = res.To<Quantity<Sum>>();
			
			auto Bru23 = extras[0] + resQ[1].To<Quantity<Sum>>();
			std::cout<<"\nBru---------------------\n:\n"<<Bru23<<std::endl;
			auto Z23 = extras[1] + resQ[2].To<Quantity<Sum>>();
			std::cout<<"\nZei---------------------\n:\n"<<Z23<<std::endl;
			std::cout<<"\n-------------------First---------------------\n:\n"<<ms[0]<<std::endl;

			assert(ms[0].To<Quantity<Sum>>()==Quantity<Sum>{-296.31}); // Waste
			assert(ms[1].To<Quantity<Sum>>()==Quantity<Sum>{-3326.63}); // Heating
			assert(ms[2].To<Quantity<Sum>>()==Quantity<Sum>{-1951.57}); // Insurance
			assert(ms[3].To<Quantity<Sum>>()==Quantity<Sum>{-918.91}); // Cleaning
			assert(ms[4].To<Quantity<Sum>>()==Quantity<Sum>{-423.00}); // Property
			assert(ms[5].To<Quantity<Sum>>()==Quantity<Sum>{-1061.32}); // Sewage
			assert(res[1].To<Quantity<Sum>>().Equals(Quantity<Sum>{-2517.51},0.01));
			assert(res[2].To<Quantity<Sum>>().Equals(Quantity<Sum>{-2678.42},0.01));
			assert(Bru23.Equals(Quantity<Sum>{-93.51},0.01));
			assert(Z23.Equals(Quantity<Sum>{-158.42},0.01));

			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
