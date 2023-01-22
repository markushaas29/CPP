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
	
    std::cout<<"Watt "<<rw<<std::endl;
	assert(rw.QuantityValue == w);
	assert(rw.Date == dt);
    return 0;
}
