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
	std::string arg = "1,3";
	std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	std::cout<<"arg_"<<arg<<" "<<std::endl;
	
	std::cout<<"1,3_"<<ParseTo<double,std::string>("1,3")<<" "<<std::endl;
	std::cout<<"1,4_"<<ParseTo<double,std::string>("1,4")<<" "<<std::endl;

//	std::cout<<"\"\"""_"<<TryTo<double>("")<<" "<<std::endl;
//	std::cout<<"1.2_"<<TryTo<double,char>(1.2)<<" "<<std::endl;
//	if(!TryTo<double,char>(1.2))
//		std::cout<<"1.2_TryTo<double,char>(1.2) false "<<std::endl;
//	std::cout<<"1.2_"<<ParseTo<double,char>(1.2)<<" "<<std::endl;

	assert(TryTo<double>("1.123"));
	assert(TryTo<uint>("123"));
	assert(!TryTo<uint>("s"));
	assert(!TryTo<uint>(""));

	std::cout<<"END"<<std::endl;
    
	return 0;
}
