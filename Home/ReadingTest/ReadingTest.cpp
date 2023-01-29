#include <iostream>
#include "../../Common/ShortNames.hpp"
#include "../../Unit/Unit.hpp"
#include "../Reading.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	using RW = Reading<Work>;
	using W = RW::QuantityType;
	
	auto w = W{123};
	auto e = Q{2};
	auto dt = DT{1,2,2003};
	auto rw = RW{w,dt};
	
	auto w2 = W{124};
	auto dt2 = DT{1,2,2004};
	auto rw2 = RW{w2,dt2};
	
	auto w3 = W{148};
	auto dt3 = DT{24,2,2004};
	auto rw3 = RW{w3,dt3};
	
    std::cout<<"Watt "<<rw<<std::endl;
	assert(rw.QuantityValue == w);
	assert(rw.Date == dt);
	
	auto drw = rw2 -rw;
	auto c = drw / e;
    //~ std::cout<<"Watt "<<drw<<std::endl;
    std::cout<<"Watt "<<drw.Get()<<std::endl;
    std::cout<<"Cons "<<c.Get()<<std::endl;
    
    auto dd = rw3.Date - rw2.Date;
    std::cout<<"Days "<<dd<<std::endl;
	auto drw2 = (rw3 -rw2) / dd;
	auto c2 = drw2 / e;
    //~ std::cout<<"Watt "<<drw<<std::endl;
    std::cout<<"Watt "<<drw2.Get()<<std::endl;
    std::cout<<"Cons "<<c2.Get()<<std::endl;
    
    auto cc = c * c;
    std::cout<<"Cons "<<cc.Get()<<std::endl;
    
	
    return 0;
}
