#include <iostream>
#include "../Reading.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Unit/Unit.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	using RW = Reading<Work>;
	using W = RW::QuantityType;
	
	auto w = W{123};
	auto dt = DT{1,2,2003};
	auto rw = RW{w,dt};
	
	auto w2 = W{124};
	auto dt2 = DT{1,2,2004};
	auto rw2 = RW{w2,dt2};
	
    std::cout<<"Watt "<<rw<<std::endl;
	assert(rw.QuantityValue == w);
	assert(rw.Date == dt);
	
	auto drw = rw2 -rw;
    //~ std::cout<<"Watt "<<drw<<std::endl;
    std::cout<<"Watt "<<drw.Get()<<std::endl;
	
    return 0;
}
