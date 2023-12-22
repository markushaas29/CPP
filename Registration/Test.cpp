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
			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date>(&fmt);

			auto qp = fmt("Sum","100");
			std::cout<<fmt<<std::endl;

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

