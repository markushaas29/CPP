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
			auto u22 = std::string{ "/home/markus/Dokumente/cpp/CSV_Files/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Dokumente/cpp/CSV_Files/U_2023.csv" };
			auto u24 = std::string{ "/home/markus/Dokumente/cpp/CSV_Files/U_2024.csv" };
			auto u25 = std::string{ "/home/markus/Dokumente/cpp/CSV_Files/U_2025.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);
			auto m24r = MatrixReader(u24);
			//auto m25r = MatrixReader(u25);
			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();
			auto m24S = m24r.M<2>();
//			auto m25S = m25r.M<2>();
			auto t = false;
			std::vector<MS2> accountFiles{m22S, m23S, m24S};//, m25S};
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

			using AllStages = std::tuple<Bottom, Middle, Top>;

			const std::string path = "/home/markus/Dokumente/cpp/CSV_Files"; 
			auto account = std::make_shared<AccountCalculator>(tokenFactory,elementFactory,visitorFactory, path);
			std::chrono::steady_clock::time_point e1 = std::chrono::steady_clock::now();

			std::cout << "Calculation Time Init :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e1 - b1).count() << "[ms]" << std::endl;
			

			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			
			auto hall = std::make_unique<Hall<HallOne>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM = (*hall)(Year(2023),bM).To<Quantity<Sum>>();
			assert(hallM[0][0]().Equals(Quantity<Sum>{504},0.01));
			assert(hallM[0][1]().Equals(Quantity<Sum>{1980},0.01));
			assert(hall->Value(Year(2023),bM).Equals(Quantity<Sum>{2484},0.01));
			
			auto hall2 = std::make_unique<Hall<HallTwo>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM2 = (*hall2)(Year(2023),bM).To<Quantity<Sum>>();
			assert(hallM2[0][0]().Equals(Quantity<Sum>{1350},0.01));
			assert(hallM2[0][1]().Equals(Quantity<Sum>{990},0.01));
			assert(hall2->Value(Year(2023),bM).Equals(Quantity<Sum>{2340},0.01));
			
			auto hall3 = std::make_unique<Hall<HallThree>>(tokenFactory,elementFactory,visitorFactory,path);
			auto hallM3 = (*hall3)(Year(2023),bM).To<Quantity<Sum>>();
			std::cout<<"Hall"<<hallM3<<std::endl;
			std::cout<<"H "<<hall2->Value(Year(2023),bM)<<std::endl;
			assert(hall3->Value(Year(2023),bM).Equals(Quantity<Sum>{2052},0.01));
			assert(hallM3[0][0]().Equals(Quantity<Sum>{2052},0.01));
			
			auto propB23 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto costs = propB23.Costs(Year{2023})[0].To<QS>();
			assert(costs[0]().Equals(Quantity<Sum>{-296.31},0.01));
			assert(costs[1]().Equals(Quantity<Sum>{-3326.63},0.01));
			assert(costs[2]().Equals(Quantity<Sum>{-1951.57},0.01));
			assert(costs[3]().Equals(Quantity<Sum>{-918.91},0.01));
			assert(costs[4]().Equals(Quantity<Sum>{-423.00},0.01));
			assert(costs[5]().Equals(Quantity<Sum>{-1061.32},0.01));

			auto bV = propB23.Values(Year{2022}, bM).To<Quantity<Sum>>();
			std::cout<<"Properties.\n"<<propB23.AdvanceItems()<<std::endl;
			assert(bV[0]().Equals(Quantity<Sum>{-107.33},0.01));
			assert(bV[1]().Equals(Quantity<Sum>{-758.60},0.01));
			assert(bV[2]().Equals(Quantity<Sum>{-417.83},0.01));
			assert(bV[3]().Equals(Quantity<Sum>{-90.00},0.01));
			assert(bV[4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bV[5]().Equals(Quantity<Sum>{-288.47},0.01));
			
			auto bMS = propB23(Year{2023}, bM).To<Quantity<Sum>>();
			auto propB = propB23.AdvanceItems()[1];
			assert(propB23.AdvancePayment().Equals(Quantity<Sum>{2424},0.01));
			assert(propB23.Result(Year{2023}).Equals(Quantity<Sum>{-93.51},0.01));
			assert(propB()[0].As<Quantity<Sum>>()==Quantity<Sum>{458});
	        assert(propB()[1].As<Quantity<Sum>>()==Quantity<Sum>{135});
          	assert(propB()[2].As<Quantity<Sum>>()==Quantity<Sum>{67});
			assert(bMS[0][0]().Equals(Quantity<Sum>{-98.77},0.01));
			assert(bMS[0][1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(bMS[0][2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(bMS[0][3]().Equals(Quantity<Sum>{-183.78},0.01));
			assert(bMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(bMS[0][5]().Equals(Quantity<Sum>{-350.66},0.01));
			assert(propB23.Value(Year{2023}).Equals(Quantity<Sum>{-2517.51},0.01));
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			auto propM23 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto mV = propM23.Values(Year{2022}, bM);
			assert(mV[0]().Equals(Quantity<Sum>{-161.00},0.01));
			assert(mV[1]().Equals(Quantity<Sum>{-758.60},0.01));
			assert(mV[2]().Equals(Quantity<Sum>{-417.83},0.01));
			assert(mV[3]().Equals(Quantity<Sum>{-90.00},0.01));
			assert(mV[4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(mV[5]().Equals(Quantity<Sum>{-352.51},0.01));
			
			auto mMS = propM23(Year{2023}, bM).To<Quantity<Sum>>();
			auto propM = propM23.AdvanceItems()[1];
			assert(propM23.AdvancePayment().Equals(Quantity<Sum>{2520},0.01));
			assert(propM23.Result(Year{2023}).Equals(Quantity<Sum>{-158.42},0.01));
			assert(propM()[0].As<Quantity<Sum>>()==Quantity<Sum>{525});
	        assert(propM()[1].As<Quantity<Sum>>()==Quantity<Sum>{0});
          	assert(propM()[2].As<Quantity<Sum>>()==Quantity<Sum>{210});
			assert(mMS[0][0]().Equals(Quantity<Sum>{-148.16},0.01));
			assert(mMS[0][1]().Equals(Quantity<Sum>{-1232.09},0.01));
			assert(mMS[0][2]().Equals(Quantity<Sum>{-487.89},0.01));
			assert(mMS[0][3]().Equals(Quantity<Sum>{-183.78},0.01));
			assert(mMS[0][4]().Equals(Quantity<Sum>{-164.32},0.01));
			assert(mMS[0][5]().Equals(Quantity<Sum>{-462.19},0.01));
			assert(propM23.Value(Year{2023}).Equals(Quantity<Sum>{-2678.42},0.01));
			

			auto propT23 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto mT22 = propT23.Values(Year{2022}, bM).To<Quantity<Sum>>();
			assert(mT22[0]().Equals(Quantity<Sum>{-53.67},0.01));
			assert(mT22[1]().Equals(Quantity<Sum>{-531.02},0.01));
			assert(mT22[2]().Equals(Quantity<Sum>{-835.65},0.01));
			assert(mT22[3]().Equals(Quantity<Sum>{-270.22},0.01));
			assert(mT22[4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mT22[5]().Equals(Quantity<Sum>{-292.31},0.01));

			auto mTS = propT23(Year{2023}, bM).To<Quantity<Sum>>();
			auto propT = propT23.AdvanceItems()[1];
			assert(propT23.AdvancePayment().Equals(Quantity<Sum>{16200},0.01));
			assert(propT23.Result(Year{2023}).Equals(Quantity<Sum>{13418.19},0.01));
			assert(propT()[0].As<Quantity<Sum>>()==Quantity<Sum>{1});
	        assert(propT()[1].As<Quantity<Sum>>()==Quantity<Sum>{1000});
          	assert(propT()[2].As<Quantity<Sum>>()==Quantity<Sum>{350});
			assert(mTS[0][0]().Equals(Quantity<Sum>{-49.38},0.01));
			assert(mTS[0][1]().Equals(Quantity<Sum>{-862.46},0.01));
			assert(mTS[0][2]().Equals(Quantity<Sum>{-975.78},0.01));
			assert(mTS[0][3]().Equals(Quantity<Sum>{-551.35},0.01));
			assert(mTS[0][4]().Equals(Quantity<Sum>{-94.36},0.01));
			assert(mTS[0][5]().Equals(Quantity<Sum>{-248.47},0.01));
			assert(propT23.Value(Year{2023}).Equals(Quantity<Sum>{-2781.81},0.01));
			
			auto propB24 = ExtraCostsCalculator<Bottom>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propM24 = ExtraCostsCalculator<Middle>(account, tokenFactory,elementFactory,visitorFactory,path);
			auto propT24 = ExtraCostsCalculator<Top>(account, tokenFactory,elementFactory,visitorFactory,path);
			
			bMS = propB23(Year{2024}, bM).To<Quantity<Sum>>();
			costs = propB23.Costs(Year{2024})[0].To<QS>();
			assert(costs[0]().Equals(Quantity<Sum>{-334.81},0.01));
//			assert(costs[1]().Equals(Quantity<Sum>{-3559.73},0.01));
//			assert(costs[2]().Equals(Quantity<Sum>{-2106.86},0.01));
//			assert(costs[3]().Equals(Quantity<Sum>{-3055},0.01));
//			assert(costs[4]().Equals(Quantity<Sum>{-423.00},0.01));
//			assert(costs[5]().Equals(Quantity<Sum>{-932},0.01));
		//	std::cout<<"Costs--->"<<bMS<<std::endl;
		//	assert(bMS[0][0]().Equals(Quantity<Sum>{-111.60},0.01));
		//	assert(bMS[0][1]().Equals(Quantity<Sum>{-1080.64},0.01));
		//	assert(bMS[0][2]().Equals(Quantity<Sum>{-526.72},0.01));
		//	assert(bMS[0][3]().Equals(Quantity<Sum>{-558.53},0.01));
		//	assert(bMS[0][4]().Equals(Quantity<Sum>{-123.24},0.01));
		//	assert(bMS[0][5]().Equals(Quantity<Sum>{-307.93},0.01));
		//	assert(propB24.Value(Year{2024}).Equals(Quantity<Sum>{-2708.66},0.01));
		//	
			mMS = propM23(Year{2024}, bM).To<Quantity<Sum>>();
		//	assert(propM24.AdvancePayment().Equals(Quantity<Sum>{2520},0.01));
		//	assert(propM24.Result(Year{2024}).Equals(Quantity<Sum>{-342.40},0.01));
		//	assert(mMS[0][0]().Equals(Quantity<Sum>{-167.40},0.01));
		//	assert(mMS[0][1]().Equals(Quantity<Sum>{-1080.64},0.01));
		//	assert(mMS[0][2]().Equals(Quantity<Sum>{-526.72},0.01));
		//	assert(mMS[0][3]().Equals(Quantity<Sum>{-558.53},0.01));
		//	assert(mMS[0][4]().Equals(Quantity<Sum>{-123.24},0.01));
		//	assert(mMS[0][5]().Equals(Quantity<Sum>{-405.87},0.01));
		//	assert(propM24.Value(Year{2024}).Equals(Quantity<Sum>{-2862.40},0.01));
		//	
			mTS = propT24(Year{2024}, bM).To<Quantity<Sum>>();
			mTS = propT24(Year{2025}, bM).To<Quantity<Sum>>();
			costs = propT24.Costs(Year{2025})[0].To<QS>();
		//	assert(propT24.AdvancePayment().Equals(Quantity<Sum>{16200},0.01));
		//	assert(propT24.Result(Year{2024}).Equals(Quantity<Sum>{12369.76},0.01));
		//	assert(mTS[0][0]().Equals(Quantity<Sum>{-55.80},0.01));
		//	assert(mTS[0][1]().Equals(Quantity<Sum>{-756.45},0.01));
		//	assert(mTS[0][2]().Equals(Quantity<Sum>{-1053.43},0.01));
		//	assert(mTS[0][3]().Equals(Quantity<Sum>{-1675.59},0.01));
		//	assert(mTS[0][4]().Equals(Quantity<Sum>{-70.77},0.01));
		//	assert(mTS[0][5]().Equals(Quantity<Sum>{-218.20},0.01));
		//	assert(propT24.Value(Year{2024}).Equals(Quantity<Sum>{-3830.24},0.01));

			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
