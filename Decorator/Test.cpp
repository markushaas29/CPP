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
			
			std::vector<int> vn = {-1,-2,3};
			std::cout<<"End Decorator"<<std::endl;

			return 0;
		}
};

