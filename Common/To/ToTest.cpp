#include <iostream>
#include <vector>
#include "../../Logger/Logger.hpp"
#include "To.hpp"

int main()
{       
	std::cout<<"1.2_"<<To("1.2")<<std::endl;
	std::cout<<"1.2_"<<To(1.2)<<std::endl;
	std::cout<<"1.2_"<<To<double,int>(1.2)<<std::endl;
	std::cout<<"1.2_"<<To<double,char>(1.2)<<std::endl;

	std::cout<<"END"<<std::endl;
    
	return 0;
}
