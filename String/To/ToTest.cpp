#include <iostream>
#include <vector>
#include "../../Logger/Logger.hpp"
#include "To.hpp"
using namespace String_;

int main()
{       
	std::cout<<"1.2_"<<ParseTo("1.2")<<std::endl;
	std::cout<<"1.2_"<<ParseTo(1.2)<<std::endl;
	std::cout<<"1.2_"<<ParseTo<double,int>(1.2)<<std::endl;
	double d = TryTo<double>("1.2");
	std::cout<<"1.2_d "<<d<<std::endl;

	std::cout<<"\"\"""_"<<TryTo<double>("")<<" "<<std::endl;
	std::cout<<"1.2_"<<TryTo<double,char>(1.2)<<" "<<std::endl;
	if(!TryTo<double,char>(1.2))
		std::cout<<"1.2_TryTo<double,char>(1.2) false "<<std::endl;
	std::cout<<"1.2_"<<ParseTo<double,char>(1.2)<<" "<<std::endl;

	std::cout<<"END"<<std::endl;
    
	return 0;
}
