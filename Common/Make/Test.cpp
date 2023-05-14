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
	auto md = Make<double>();
	auto i12 = md(is);
	std::cout<<i12<<std::endl;
	assert(i12.Value==12.0);
	assert((double)i12==12.0);
	std::cout<<"END"<<std::endl;
    
	std::istringstream isd("12 12 2011");
	auto mda = Make<DateTimes::Date>();
	auto id = mda(isd);
	std::cout<<id<<std::endl;
	
	std::istringstream isd2("12 12 2011");
	auto mda2 = Make<DateTimes::Date>()(isd2);
	return 0;
}
