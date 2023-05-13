#include <iostream>
#include <vector>
#include <sstream>
#include "../../Logger/Logger.hpp"
#include "../DateTimes.hpp"
#include "Make.hpp"

int main()
{       
	std::string arg = "1,3";
	std::istringstream is("12");

	auto i12 = Make<double>(is);
	std::cout<<i12<<std::endl;
	assert(i12.Value==12.0);
	assert((double)i12==12.0);
	std::cout<<"END"<<std::endl;
    
	std::istringstream isd("12 12 2011");
	auto id = Make<DateTimes::Date>(isd);
	std::cout<<id<<std::endl;
	return 0;
}
