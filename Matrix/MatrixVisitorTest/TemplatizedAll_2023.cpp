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
			
			auto extra_Bottom = ExtraCostItemsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory, Year{2023},path};
			auto pBottom = ProportionCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory, Year{2023},path};
			HtmlBuilder<German> hb(path);
			std::cout<<"P--->"<<pBottom(hb)<<std::endl;
			assert(extra_Bottom.Value(hb).Equals(Quantity<Sum>{660},0.01));
			auto extra_Middle = ExtraCostItemsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extra_Top = ExtraCostItemsCalculator<Top>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extraCostsBottom = YearlyExtraCostsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			assert(extraCostsBottom.Value(hb).Equals(Quantity<Sum>{2424},0.01));
			auto extraCostsMiddle = YearlyExtraCostsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			std::cout<<"ItemsCalculator--->"<<extra_Bottom<<std::endl;
			std::cout<<"Costs--->"<<extraCostsBottom<<std::endl;

			auto inv = std::make_unique<Invoice<Bottom>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto b = inv->calcAll<0,AllStages>(mps,tokenFactory,elementFactory,visitorFactory,path);
			assert(inv->Costs()[0]==Quantity<Sum>{-296.31});
			std::cout<<"Prop--->"<<inv->Costs()<<std::endl;
			assert(inv->Costs()[1]==Quantity<Sum>{-3326.63});
			assert(b.Equals(Quantity<Sum>{-93.51},0.01));
			std::cout<<"Prop--->"<<inv->Prop()<<std::endl;
			assert(inv->Prop()[3].As<Quantity<Area>>()==Quantity<Area>{100});
			(*inv)(hb);

			auto m = inv->calcAll<1,AllStages>(mps,tokenFactory,elementFactory,visitorFactory, path);
			std::cout<<"ALL Res--->"<<m<<std::endl;
			assert(m.Equals(Quantity<Sum>{-158.42},0.01));

			assert(inv->Prop()[8].As<Quantity<Sum>>()==Quantity<Sum>{458});
	        assert(inv->Prop()[9].As<Quantity<Sum>>()==Quantity<Sum>{135});
          	assert(inv->Prop()[10].As<Quantity<Sum>>()==Quantity<Sum>{67});
//          	std::cout<<"MS sum:\n"<<inv->Prop().Accumulate(1,3)*12<<std::endl;
//          	assert(inv->Prop().Accumulate(1,3)*12==Quantity<Sum>{2424});
			
			std::chrono::steady_clock::time_point e2 = std::chrono::steady_clock::now();
			std::cout << "Calculation Time Bottom  :"<< std::chrono::duration_cast<std::chrono::milliseconds> (e2 - e1).count() << "[ms]" << std::endl;

			assert(inv->Value(hb).Equals(Quantity<Sum>{-93.51},0.01));
			auto invM = std::make_unique<Invoice<Middle>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			assert(invM->Value(hb).Equals(Quantity<Sum>{-158.42},0.01));
			assert(invM->Prop()[8].As<Quantity<Sum>>()==Quantity<Sum>{525});
	        assert(invM->Prop()[9].As<Quantity<Sum>>()==Quantity<Sum>{0});
          	assert(invM->Prop()[10].As<Quantity<Sum>>()==Quantity<Sum>{210});
			auto ibq = (*invM)(hb);
			
			auto invT = std::make_unique<Invoice<Top>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			std::cout<<"END 2023"<<invT->Prop()<<std::endl;
			assert(invT->Prop()[8].As<Quantity<Sum>>()==Quantity<Sum>{1});
	        assert(invT->Prop()[9].As<Quantity<Sum>>()==Quantity<Sum>{1000});
          	assert(invT->Prop()[10].As<Quantity<Sum>>()==Quantity<Sum>{350});
			auto itq = (*invT)(hb);
			std::cout<<"END 2023"<<itq<<std::endl;

			auto invM24 = std::make_unique<Invoice<Middle>>(tokenFactory,elementFactory,visitorFactory,Year{2024},path);
			auto bM = HtmlBuilder<German>("Middle_T_2003.html");
			auto propM23 = ProportionCalculator<Middle>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			propM23(bM,Year{2023});
			std::cout<<"M23"<<invM->Costs()<<std::endl;
			std::cout<<"M24"<<invM24->Costs()<<std::endl;
			//assert(invM24->Value().Equals(Quantity<Sum>{-158.42},0.01));
			assert(invM24->Prop()[8].As<Quantity<Sum>>()==Quantity<Sum>{525});
	        assert(invM24->Prop()[9].As<Quantity<Sum>>()==Quantity<Sum>{0});
          	assert(invM24->Prop()[10].As<Quantity<Sum>>()==Quantity<Sum>{210});


			auto hall = std::make_unique<Hall<Middle>>(tokenFactory,elementFactory,visitorFactory,Year{2024},path);
			std::cout<<"Stage"<<mS<<std::endl;
			std::cout<<"Hall"<<(*hall)(hb)<<std::endl;
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
