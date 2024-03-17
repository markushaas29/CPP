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
#include "../../Invoice/Invoice.hpp"

class MatrixVisitorTest2023
{
	template<size_t N, typename Tup>
	auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y, const std::string& p)
	{
	    if constexpr (std::tuple_size<Tup>()==N)
	        return stageMatrix;
	    else
	    {   
	        using Type = std::tuple_element_t<N,Tup>;
	        auto readings = Readings<Type>{fT,fE,fB, y, p};
	        stageMatrix = stageMatrix.Set(readings()[0].template As<Quantity<Scalar>>(),Type::Index,((int)stageMatrix.Cols()-1));
	        return process<N+1,Tup>(stageMatrix,fT,fE,fB,y,p);
	    }
	}

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

            auto tokenFactory = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken,IBANIndexToken, BICIndexToken, NameIndexToken, SumIndexToken, UseIndexToken, DateIndexToken, StageIndexToken, WasteIndexToken, HeatingIndexToken, CleaningIndexToken, SewageIndexToken, PropertyTaxIndexToken, InsuranceIndexToken, RentIndexToken, HeatExtraCostIndexToken, ExtraCostIndexToken, GarageRentIndexToken>();
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
            auto reg3 = Registration<Factory<BaseVisitor>,AccumulationVisitor<>,AccumulationVisitor<Quantity<Volume>>,ConsumptionVisitor<Quantity<Volume>>>(&(*visitorFactory));

			auto mps = mS.Match(smatcher).Parse(matcher).Cols(2,3,4,5,6,7).To<Quantity<Scalar>>();
			auto stageQ = mS.Match(smatcher).Parse(matcher);
			auto payment = stageQ.Cols(8,9,10).To<Quantity<Sum>>();

			using AllStages = std::tuple<Bottom, Middle, Top>;

			const std::string path = "/home/markus/Downloads/CSV_TestFiles_2"; 
			auto account = AccountCalculator{tokenFactory,elementFactory,visitorFactory, Year{2023},path};
			
			auto extra_Bottom = ExtraCostItemsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extra_Middle = ExtraCostItemsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extra_Top = ExtraCostItemsCalculator<Top>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extraCostsBottom = YearlyExtraCostsCalculator<Bottom>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};
			auto extraCostsMiddle = YearlyExtraCostsCalculator<Middle>{tokenFactory,elementFactory,visitorFactory,Year{2023},path};

			mps = process<0,AllStages>(mps,tokenFactory,elementFactory,visitorFactory,Year{2023},path);
			assert(mps[1][5].To<Quantity<Scalar>>().Equals(Quantity<Scalar>{0.3304},0.01));
			assert(mps[2][5].To<Quantity<Scalar>>().Equals(Quantity<Scalar>{0.4354},0.01));
			assert(mps[3][5].To<Quantity<Scalar>>().Equals(Quantity<Scalar>{0.23411},0.01));

			auto pay = account();
			auto ms = pay.To<Quantity<Sum>>();

			auto mpsM = (mps / mps.ColSum());
			auto res = mpsM * ms;

			assert(extra_Bottom()[0].As<Quantity<Sum>>()==Quantity<Sum>{458});
			assert(extra_Bottom()[1].As<Quantity<Sum>>()==Quantity<Sum>{135});
			assert(extra_Bottom()[2].As<Quantity<Sum>>()==Quantity<Sum>{67});
			std::cout<<"MS sum:\n"<<extra_Bottom.Accumulate(1,3)*12<<std::endl;
			assert(extra_Bottom.Accumulate(1,3)*12==Quantity<Sum>{2424});
			
			assert(extra_Middle()[0].As<Quantity<Sum>>()==Quantity<Sum>{525});
			assert(extra_Middle()[1].As<Quantity<Sum>>()==Quantity<Sum>{0});
			assert(extra_Middle()[2].As<Quantity<Sum>>()==Quantity<Sum>{210});
			assert(extra_Middle.Accumulate(1,3)*12==Quantity<Sum>{2520});
			
//			assert(extra_Top()[0].As<Quantity<Sum>>()==Quantity<Sum>{1});
//			assert(extra_Top()[1].As<Quantity<Sum>>()==Quantity<Sum>{1});
//			assert(extra_Top()[2].As<Quantity<Sum>>()==Quantity<Sum>{1});
			auto ecb = extraCostsBottom()[0].As<Quantity<Sum>>();
			auto ecm = extraCostsMiddle()[0].As<Quantity<Sum>>();
			assert(ecb==Quantity<Sum>{2424});
			assert(ecm==Quantity<Sum>{2520});

			auto resQ = res.To<Quantity<Sum>>();
			auto Bru23 = ecb + resQ[1].To<Quantity<Sum>>();
			auto Z23 = ecm + resQ[2].To<Quantity<Sum>>();

//			std::cout<<"MS sum:\n"<<ms.To<Quantity<Sum>>().ColSum()<<std::endl;
			
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
