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

class TemplatizedAll_Test2023
{
	public:
		int Run()
		{
			std::cout<<"START TemplatizedAll_ 2023"<<std::endl;
			auto b1 = std::chrono::steady_clock::now();
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

            auto tokenFactory = TokenFactoryCreator()();
            auto stageIndexTokens = (*tokenFactory)({{"NameIndexToken"},{"StageIndexToken"},{"WasteIndexToken"},{"HeatingIndexToken"},{"CleaningIndexToken"},{"SewageIndexToken"},{"PropertyTaxIndexToken"},{"InsuranceIndexToken"},{"RentIndexToken"},{"ExtraCostsIndexToken"},{"HeatExtraCostsIndexToken"} });
            Matcher smatcher(std::move(stageIndexTokens));
            auto csvIndexTokens = (*tokenFactory)({{"SumIndexToken"},{"IBANIndexToken"},{"DateIndexToken"},{"BICIndexToken"},{"NameIndexToken"}, {"VerwendungszweckIndexToken"}});
            Matcher imatcher(std::move(csvIndexTokens));

            auto v = (*tokenFactory)({{"SumToken"},{"IBANToken"},{"DateToken"},{"EmptyToken"},{"ValueToken"},{"EntryToken"},{"ScalarToken"}});
            Matcher matcher(std::move(v));

			auto parsedAccountMatrix = accountMatrix.Parse(imatcher, matcher);

            auto elementFactory = Build<IElement,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Year, Month,Index<int>, Entry,Empty>();
            auto typeFactory = std::make_shared<TF>(elementFactory);

			auto visitorFactory = std::make_shared<Factory<BaseVisitor>>();
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));

			auto mps = mS.Parse(smatcher, matcher).Cols(2,3,4,5,6,7).To<Quantity<Scalar>>();
			auto stageQ = mS.Parse(smatcher, matcher);
			auto payment = stageQ.Cols(8,9,10).To<Quantity<Sum>>();

			stageQ.CSV();

			using AllStages = std::tuple<Bottom, Middle, Top>;

			const std::string path = "/home/markus/Downloads/CSV_TestFiles_2"; 
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			

			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			auto propB23 = ProportionCalculator<Bottom>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto bMS = propB23(bM,Year{2023}).To<Quantity<Sum>>();
			auto propB = propB23.AdvanceItems()[1];
			assert(propB23.AdvancePayment().Equals(Quantity<Sum>{2424},0.01));
			assert(propB23.Result().Equals(Quantity<Sum>{-93.51},0.01));
			std::cout<<"Costs--->"<<propB()[0]<<std::endl;
			assert(propB()[0].As<Quantity<Sum>>()==Quantity<Sum>{458});
	        assert(propB()[1].As<Quantity<Sum>>()==Quantity<Sum>{135});
          	assert(propB()[2].As<Quantity<Sum>>()==Quantity<Sum>{67});
			assert(bMS[0]().Equals(Quantity<Sum>{-98.77},0.01));
			assert(bMS[1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(bMS[2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(bMS[3]().Equals(Quantity<Sum>{-183.78},0.01));
			assert(bMS[4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bMS[5]().Equals(Quantity<Sum>{-350.66},0.01));
			assert(propB23.Value().Equals(Quantity<Sum>{-2517.51},0.01));
			
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			auto propM23 = ProportionCalculator<Middle>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto mMS = propM23(bM,Year{2023}).To<Quantity<Sum>>();
			auto propM = propM23.AdvanceItems()[1];
			assert(propM23.AdvancePayment().Equals(Quantity<Sum>{2520},0.01));
			assert(propM23.Result().Equals(Quantity<Sum>{-158.42},0.01));
			assert(propM()[0].As<Quantity<Sum>>()==Quantity<Sum>{525});
	        assert(propM()[1].As<Quantity<Sum>>()==Quantity<Sum>{0});
          	assert(propM()[2].As<Quantity<Sum>>()==Quantity<Sum>{210});
			assert(mMS[0]().Equals(Quantity<Sum>{-148.16},0.01));
			assert(mMS[1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(mMS[2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(mMS[3]().Equals(Quantity<Sum>{-183.78},0.01));
			assert(mMS[4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(mMS[5]().Equals(Quantity<Sum>{-462.19},0.01));
			assert(propM23.Value().Equals(Quantity<Sum>{-2678.42},0.01));

			auto propT23 = ProportionCalculator<Top>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto mTS = propT23(bM,Year{2023}).To<Quantity<Sum>>();
			auto propT = propT23.AdvanceItems()[1];
			std::cout<<"END 2023 "<<propT23.AdvancePayment()<<std::endl;
			assert(propT23.AdvancePayment().Equals(Quantity<Sum>{16200},0.01));
			assert(propT23.Result().Equals(Quantity<Sum>{13418.19},0.01));
			assert(propT()[0].As<Quantity<Sum>>()==Quantity<Sum>{1});
	        assert(propT()[1].As<Quantity<Sum>>()==Quantity<Sum>{1000});
          	assert(propT()[2].As<Quantity<Sum>>()==Quantity<Sum>{350});
			assert(mTS[0]().Equals(Quantity<Sum>{-49.38},0.01));
			assert(mTS[1]().Equals(Quantity<Sum>{-862.46},0.01));
			assert(mTS[2]().Equals(Quantity<Sum>{-975.78},0.01));
			assert(mTS[3]().Equals(Quantity<Sum>{-551.35},0.01));
			assert(mTS[4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mTS[5]().Equals(Quantity<Sum>{-248.47},0.01));
			assert(propT23.Value().Equals(Quantity<Sum>{-2781.81},0.01));

			HtmlBuilder<German> hb(path);

			auto hall = std::make_unique<Hall<Middle>>(tokenFactory,elementFactory,visitorFactory,Year{2024},path);
			std::cout<<"Stage"<<mS<<std::endl;
			std::cout<<"Hall"<<(*hall)(hb)<<std::endl;
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
