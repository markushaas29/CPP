#include <iostream>
#include "../Direction.hpp"
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
    
	auto d = Direction("Hs");
    std::cout<<"D: "<<d<<std::endl;
    std::cout<<"D id: "<<d.QuantityValue()<<std::endl;
    
    std::cout<<in->QuantityValue()<<std::endl;
    std::cout<<out->QuantityValue()<<std::endl;
    std::cout<<un->QuantityValue()<<std::endl;
    std::cout<<(un->QuantityValue() * in->QuantityValue())<<std::endl;
	
    return 0;
}
