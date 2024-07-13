#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "CounterHeaders.hpp"
#include "CounterDescription.hpp"
#include "../Builder/Builder.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../Unit/Unit.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Visitor/CollectorVisitor.hpp"

class CounterTest
{
	public:
		int Run()
		{
			std::cout<<"START COunterTest"<<std::endl;
			auto tf = TokenFactoryCreator()();
			auto wd = TopHotDesc{tf,"/home/markus/Downloads/CSV_TestFiles_2"};
			std::cout<<"CABSter\n"<<wd<<std::endl;

			auto c = std::make_unique<Counter<TopHotDesc>>("/home/markus/Downloads/CSV_TestFiles_2",tf);

			std::cout<<"Counter\n"<<*c<<std::endl;
			std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
			civ = c->Accept(std::move(civ));
            auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
			std::cout<<"Counter\n"<<*(consV())<<std::endl;

//			auto tfd = TypeFactory<Factory<IDescription>, GasDesc>();
//			auto dv = tfd({{"Gas_House_1202757","/home/markus/Downloads/CSV_TestFiles_2"}});
			auto dv = GasDesc{tf,"/home/markus/Downloads/CSV_TestFiles_2"};
			std::cout<<"Counter\n"<<dv<<std::endl;

			Builder<ICounter,Counter,TopHotDesc, TopColdDesc, MiddleHotDesc, MiddleColdDesc, BottomHotDesc, BottomColdDesc> b;
			auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tf);
			std::cout<<"cv"<<cV->size()<<std::endl;
			std::for_each(cV->begin(), cV->end(), [&](const auto& i){ std::cout<<*i<<std::endl;  });
			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
