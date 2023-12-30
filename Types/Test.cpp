#include <iostream>
#include <cassert> 
#include "Types.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Token.hpp"
#include "../Quantity/Quantity.hpp"

class TypesTest
{
	public:
		int Run()
		{
			std::cout<<"START Types"<<std::endl;


			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>(&(*fmt));

			std::cout<<"END Types"<<std::endl;

			return 0;
		}
};

