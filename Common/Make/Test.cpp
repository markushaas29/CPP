#include <iostream>
#include <vector>
#include <sstream>
#include "../../Logger/Logger.hpp"
#include "Make.hpp"

int main()
{       
	std::string arg = "1,3";
	std::istringstream is("12");

	Make<double>(arg);
	auto i12 = Make<double>(is);
	assert(i12==12.0);
	std::cout<<"END"<<std::endl;
    
	return 0;
}
