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
			auto sNew = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN_Name.csv" };
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
			auto account = Account{tokenFactory,elementFactory,visitorFactory, path};
			auto stages = Stages{tokenFactory,elementFactory,visitorFactory, path};
			auto extrasC = ExtraCosts{tokenFactory,elementFactory,visitorFactory, path};
			auto rT = readingsT();
			auto rM = readingsM();
			auto rB = readingsB();

			int c = mps.Cols()-1;
	        mps = mps.Set(rT[0].As<Quantity<Scalar>>(),Bottom::Index,c);
	        mps = mps.Set(rM[0].As<Quantity<Scalar>>(),Middle::Index,c);
	        mps = mps.Set(rB[0].As<Quantity<Scalar>>(),Top::Index,c);
			std::cout<<"\n-------------------account MPS---------------------\n:\n"<<mps<<std::endl;

			auto pay = account();
			auto ms = pay.To<Quantity<Sum>>();

			auto mpsM = (mps / mps.ColSum());
			auto res = mpsM * ms;
			std::cout<<"\n-------------------MPS Zei Result() =---------------------\n:\n"<<stages()<<std::endl;
			auto resQ = res.To<Quantity<Sum>>();
			
			auto Bru23 = extras[0] + resQ[1].To<Quantity<Sum>>();
			std::cout<<"\nBru---------------------\n:\n"<<Bru23<<std::endl;
			auto Z23 = extras[1] + resQ[2].To<Quantity<Sum>>();
			std::cout<<"\nZei---------------------\n:\n"<<Z23<<std::endl;
			std::cout<<"\n-------------------First---------------------\n:\n"<<ms[0]<<std::endl;
			std::cout<<"\n-------------------EXTRAS MPS---------------------\n:\n"<<extrasC()<<std::endl;

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
