#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixProjector.hpp"
#include "../Matrix/MatrixFilter.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/MatrixElement.hpp"
#include "../Matrix/M3.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"
#include "../Invoice/Invoice.hpp"
#include "../Invoice/IAccountQuery.hpp"
#include "../TypeCounter/TypeCounter.hpp"
#include "2022.hpp"
#include "2023.hpp"
#include "2024.hpp"
#include "2025.hpp"
#include "2026.hpp"

class HouseTest
{
	public:
		//int Run(const auto& tokenFactory, const auto& elementFactory,const auto& visitorFactory,const auto& path)
		int Run()
		{
			std::cout<<"START HOUSE"<<std::endl;
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
			
			auto hC = std::make_unique<HallCurrentCalculator>(tokenFactory,elementFactory,visitorFactory,path);
			
			auto hC23 = (*hC)(Year(2023),bM).To<Quantity<Sum>>();
			assert(hC->Value(Year{2023}).Equals(Quantity<Sum>{-124.31},0.01));
			
			auto hC24 = (*hC)(Year(2024),bM).To<Quantity<Sum>>();
			assert(hC24[0][0]().Equals(Quantity<Sum>{-202.01},0.01));
			
			auto hC25 = (*hC)(Year(2025),bM).To<Quantity<Sum>>();
			assert(hC25[0][0]().Equals(Quantity<Sum>{-227.73},0.01));
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			
			Invoice_2026().Run(tokenFactory,elementFactory,visitorFactory,path);
			Invoice_2025().Run(tokenFactory,elementFactory,visitorFactory,path);
			Invoice_2024().Run(tokenFactory,elementFactory,visitorFactory,path);
			Invoice_2023().Run(tokenFactory,elementFactory,visitorFactory,path);
			Invoice_2022().Run(tokenFactory,elementFactory,visitorFactory,path);

			std::cout<<"END HOUSE"<<std::endl;
		   
			return 0;
		}
};
