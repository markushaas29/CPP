#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "TypeRegistration.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../ObjectFactory/Factory.hpp"

class TypeRegistrationTest
{
	public:
		int Run()
		{
			std::cout<<"START TypeRegistration"<<std::endl;

			Registration<IBAN> reg;

			Factory<IElement> fmt;
    		fmt.Register("Q",[](const std::string& s) { return std::make_unique<Quantity<Sum>>(s); });
    		fmt.Register("IBAN",[](const std::string& s) { return std::make_unique<IBAN>(s); });

			auto qp = fmt("Q","100");
			std::cout<<*qp<<std::endl;

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

