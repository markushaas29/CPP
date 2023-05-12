#include <iostream>
#include <vector>
#include <sstream>
#include "../../Logger/Logger.hpp"
#include "Make.hpp"

int main()
{       
	std::string arg = "1,3";

	Make<double>(arg);
	Make<double>(std::cin);
	std::cout<<"END"<<std::endl;
    
	return 0;
}
