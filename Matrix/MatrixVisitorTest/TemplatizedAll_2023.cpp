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

            auto tokenFactory = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken,IBANIndexToken, BICIndexToken, NameIndexToken, SumIndexToken, UseIndexToken, DateIndexToken, StageIndexToken, WasteIndexToken, HeatingIndexToken, CleaningIndexToken, SewageIndexToken, PropertyTaxIndexToken, InsuranceIndexToken, RentIndexToken, HeatExtraCostIndexToken, ExtraCostIndexToken, GarageRentIndexToken>();
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
			
			auto extra_Bottom = ExtraCostItemsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory, Year{2023},path};
			auto pBottom = ProportionCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory, Year{2023},path};
			std::cout<<"P--->"<<pBottom()<<std::endl;
			assert(extra_Bottom.Value().Equals(Quantity<Sum>{660},0.01));
			auto extra_Middle = ExtraCostItemsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extra_Top = ExtraCostItemsCalculator<Top>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extraCostsBottom = YearlyExtraCostsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			assert(extraCostsBottom.Value().Equals(Quantity<Sum>{2424},0.01));
			auto extraCostsMiddle = YearlyExtraCostsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			std::cout<<"ItemsCalculator--->"<<extra_Bottom<<std::endl;
			std::cout<<"Costs--->"<<extraCostsBottom<<std::endl;

			auto inv = std::make_unique<Invoice<Bottom>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto b = inv->calcAll<0,AllStages>(mps,tokenFactory,elementFactory,visitorFactory,path);
			assert(inv->Costs()[0]==Quantity<Sum>{-296.31});
			assert(inv->Costs()[1]==Quantity<Sum>{-3326.63});
			assert(b.Equals(Quantity<Sum>{-93.51},0.01));
			std::cout<<"Prop--->"<<inv->Prop()<<std::endl;
			assert(inv->Prop()[3].As<Quantity<Area>>()==Quantity<Area>{100});

			auto m = inv->calcAll<1,AllStages>(mps,tokenFactory,elementFactory,visitorFactory, path);
			std::cout<<"ALL Res--->"<<m<<std::endl;
			assert(m.Equals(Quantity<Sum>{-158.42},0.01));


			assert(inv->Value().Equals(Quantity<Sum>{-93.51},0.01));
			auto invM = std::make_unique<Invoice<Middle>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			assert(invM->Value().Equals(Quantity<Sum>{-158.42},0.01));
			auto ibq = (*invM)();
			std::cout<<"END 2023"<<ibq<<std::endl;
			
			auto invT = std::make_unique<Invoice<Top>>(tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			auto itq = (*invT)();
			std::cout<<"END 2023"<<itq<<std::endl;
			std::cout<<"Stage"<<mS<<std::endl;
			TypeCounts<Date,IBAN,BIC,Year,Quantity<Sum>, Quantity<Volume>>::Exec<0>();
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
