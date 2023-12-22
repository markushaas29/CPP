#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "TypeRegistration.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

class TypeRegistrationTest
{
	public:
		int Run()
		{
			std::cout<<"START TypeRegistration"<<std::endl;

//			TypeRegistration::Get().Is();

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

