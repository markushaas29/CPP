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

class MatrixReaderTest
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
			auto c = std::string{ "/home/markus/Dokumente/cpp/CSV_Files/Comdirect.csv" };
			auto m = MatrixReader(c).M<2>();
			std::cout<<m<<std::endl;
			auto t = false;

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
			auto cm = AccountParser2::Instance(tokenFactory,"/home/markus/Dokumente/cpp/CSV_Files")();
			std::cout<<cm<<std::endl;
//			using AllStages = std::tuple<Bottom, Middle, Top>;
			std::cout<<"END 2023"<<std::endl;
		   
			return 0;
		}
};
