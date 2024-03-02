#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Decorator.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

class DecoratorTest
{
	public:
		int Run()
		{
			std::cout<<"Start Decorator"<<std::endl;
			using QM = Quantity<Mass>;
			using QV = Quantity<Volume>;
			
			//{"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}}}}
			std::vector<std::string> v = { "Entry", "Abschlagsforderung"};

			FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>> fu = {"Deduction",{{"EqualVisitor", { "IBAN", "DE44600501010008017284"}}}};
			auto dy = Decorator(Year{2023},fu);
			
			std::cout<<dy<<std::endl;
			assert(fu.Units().size() == 1);
			auto fud = dy();
			assert(fud.Units().size() ==2);

			std::cout<<fud<<std::endl;
			std::cout<<"End Decorator"<<std::endl;

			return 0;
		}
};

