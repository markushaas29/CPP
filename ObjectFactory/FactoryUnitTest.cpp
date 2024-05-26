#include <iostream>
#include "Factory.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/IMatrixQuery.hpp"
#include "../Matrix/MatrixQuery.hpp"
#include "../Matrix/MatrixCategory.hpp"
#include "../Matrix/MatrixMultiCategory.hpp"

class FactoryUnitTest
{
	public:
int Run()
{
	std::cout<<"START FactoryUnit"<<std::endl;
	FactoryUnit fu;
	std::istringstream is("1 2 3.4 km");
	is>>fu;
	assert(fu.Id()=="1");
	assert(fu.Arg()=="2");
	std::cout<<"factory "<<fu<<std::endl;

	std::vector<FactoryUnitContainer<std::vector<FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>>> allFactoryUnits = 
        {
            {"Waste",
                {
                
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE44600501010008017284"}}, {EqualVisitor::Identifier, { Year::Identifier, "2023"}}}} // Waste
                }
            }, 
            {"Heating",
                {
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Abschlagsforderung"}}, {EqualVisitor::Identifier, { Entry::Identifier, "701006843905"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}}}
                }
            },
            {"BuildingInsurance",{
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE97500500000003200029"}}, {EqualVisitor::Identifier, { Year::Identifier, "2023"}}}} // Insurance
                }
            },
        };
   FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>> fuc =
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE44600501010008017284"}}, {EqualVisitor::Identifier, { Year::Identifier, "2023"}}}};
   
   FactoryUnit<std::string,FactoryUnit<std::string, std::string>> ffu = {EqualVisitor::Identifier, { IBAN::Identifier, "DE44600501010008017284"}};
	std::cout<<"fuc"<<fuc<<std::endl;
	std::cout<<"fuc"<<ffu<<std::endl;
	std::cout<<"END factory"<<std::endl;
	return 0;
}
};
