#include <iostream>
#include "Direction.hpp"
using namespace Bank;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	auto out = Direction::Create('S');
	auto in = Direction::Create('H');
	auto un = Direction::Create('s');
	
    std::cout<<out->Sign()<<std::endl;
    std::cout<<in->Sign()<<std::endl;
    std::cout<<un->Sign()<<std::endl;
	
    return 0;
}
