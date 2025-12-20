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
#include "../MatrixElement.hpp"
#include "../M3.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Element.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"
#include "../../Invoice/Invoice.hpp"
#include "../../TypeCounter/TypeCounter.hpp"

#pragma once

class Invoice_2025
{
	public:
		int Run()
		{
			std::cout<<"START 2025"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MS2 = Matrix<2,MDS2>;
		    using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
			using EVF = Factory<BaseVisitor>;

            auto tokenFactory = TokenFactoryCreator()();
            auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"} });
            Matcher smatcher(std::move(stageIndexTokens));
            auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
            Matcher imatcher(std::move(csvIndexTokens));

            auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
            Matcher matcher(std::move(v));

            auto elementFactory = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Month,Index<int>, Entry,Empty>();
            auto typeFactory = std::make_shared<TF>(elementFactory);

			auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));

			using AllStages = std::tuple<Bottom, Middle, Top>;

			const std::string path = "/home/markus/Dokumente/cpp/CSV_Files"; 
			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM = (*hall)(Year(2025),bM).To<Quantity<Sum>>();
			assert(hallM[0][0]().Equals(Quantity<Sum>{680.40},0.01));
			assert(hallM[0][1]().Equals(Quantity<Sum>{2653.2},0.01));
			assert(hall->Value(Year(2025),bM).Equals(Quantity<Sum>{3333.6},0.01));
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			(*hall2)(Year(2024),bM).To<Quantity<Sum>>();
			auto hallM2 = (*hall2)(Year(2025),bM).To<Quantity<Sum>>();
//			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
//			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
//			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM3 = (*hall3)(Year(2025),bM).To<Quantity<Sum>>();
			std::cout<<"Hall"<<hallM3<<std::endl;
			std::cout<<"H "<<hall2->Value(Year(2025),bM)<<std::endl;
			assert(hall3->Value(Year(2025),bM).Equals(Quantity<Sum>{2770.20},0.01));
			assert(hallM3[0][0]().Equals(Quantity<Sum>{2770.20},0.01));
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			auto propB24 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propM24 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propT24 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);

			auto mB2025 = propB24(Year{2025}, bM).To<Quantity<Sum>>();
			std::cout<<"mB "<<mB2025[0][4]<<std::endl;
			assert(mB2025[0][0]().Equals(Quantity<Sum>{-115.45},0.01));
//			assert(mB2025[0][1]().Equals(Quantity<Sum>{-115.45},0.01));
//			assert(mB2025[0][2]().Equals(Quantity<Sum>{-115.45},0.01));
			assert(mB2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mB2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
			//assert(mB2025[0][5]().Equals(Quantity<Sum>{-252.59},0.01));
//			assert(mB2025[0][6]().Equals(Quantity<Sum>{-115.45},0.01));
//
			auto mM2025 = propM24(Year{2025}, bM).To<Quantity<Sum>>();
			assert(mM2025[0][0]().Equals(Quantity<Sum>{-173.18},0.01));
//			assert(mM2025[0][1]().Equals(Quantity<Sum>{-173.18},0.01));
//			assert(mM2025[0][2]().Equals(Quantity<Sum>{-173.18},0.01));
			assert(mM2025[0][3]().Equals(Quantity<Sum>{-629.76},0.01));
			assert(mM2025[0][4]().Equals(Quantity<Sum>{-252.59},0.01));
//			assert(mM2025[0][5]().Equals(Quantity<Sum>{-252,59.18},0.01));
//			assert(mM2025[0][6]().Equals(Quantity<Sum>{-629.76},0.01));
//
			auto mT2025 = propT24(Year{2025}, bM).To<Quantity<Sum>>();
			std::cout<<"mB "<<mT2025<<std::endl;
			assert(mT2025[0][0]().Equals(Quantity<Sum>{-57.73},0.01));
//			assert(mT2025[0][1]().Equals(Quantity<Sum>{-57.73},0.01));
//			assert(mT2025[0][2]().Equals(Quantity<Sum>{-57.73},0.01));
			assert(mT2025[0][3]().Equals(Quantity<Sum>{-1889.28},0.01));
			assert(mT2025[0][4]().Equals(Quantity<Sum>{-145.05},0.01));
//			assert(mT2025[0][5]().Equals(Quantity<Sum>{-57.73},0.01));
//			assert(mT2025[0][6]().Equals(Quantity<Sum>{-1889.28},0.01));
			
			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
			
			auto hC23 = (*hC)(Year(2023),bM).To<Quantity<Sum>>();
			assert(hC->Value(Year{2023}).Equals(Quantity<Sum>{-124.31},0.01));
			
			auto hC24 = (*hC)(Year(2024),bM).To<Quantity<Sum>>();
			assert(hC24[0][0]().Equals(Quantity<Sum>{-202.01},0.01));
			
			auto hC25 = (*hC)(Year(2025),bM).To<Quantity<Sum>>();
			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			
			std::cout<<"END 2025"<<std::endl;
		   
			return 0;
		}
};
