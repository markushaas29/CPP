#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Registration.hpp"
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


			Factory<IElement> fmt;
			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN>(&fmt);
    		fmt.Register("IBAN",[](const std::string& s) { return std::make_unique<IBAN>(s); });

			auto qp = fmt("Q","100");
			std::cout<<fmt<<std::endl;

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

