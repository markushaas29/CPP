#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Decorator.hpp"
#include "ElementDecorator.hpp"
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
			std::unique_ptr<IFactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>> pfu = std::make_unique<FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>(fu);
			auto dy = Decorator(Year{2023},fu);
			auto dyp = Decorator(Year{2023},std::move(pfu));
			
			std::cout<<dy<<std::endl;
			assert(fu.Units().size() == 1);
			auto fud = dy();
			assert(fud->Units().size() ==2);

			std::cout<<*fud<<std::endl;

			auto b = Bold();
			auto r = Red();
			auto qm = QM{5};

			std::cout<<"B"<<r(b(qm.Data()))<<std::endl;

			std::cout<<"End Decorator"<<std::endl;

			return 0;
		}
};

