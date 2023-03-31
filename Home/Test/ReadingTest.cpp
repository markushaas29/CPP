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
	assert(w2!=w3);
	assert(w2<w3);
	assert(w3>w2);
	assert(w2!=w3);
	auto dt3 = DT{24,2,2004};
	auto rw3 = RW{w3,dt3};
	
    std::cout<<"Watt "<<rw<<std::endl;
	assert(rw.Value() == w);
	assert(rw.Date() == dt);
	
	auto drw = rw2 -rw;
	auto drwq = rw2.Value() -rw.Value();
    auto subD = (rw2.Date() - rw.Date());
    //~ auto subD = (rw2.Date - rw.Date);
	//~ auto c = (drw / subD) ;/// e;
	//~ (int)drwq;
	auto c = (drw / subD) ;
    //~ std::cout<<"Watt "<<drw<<std::endl;
    std::cout<<"Q Watt "<<drwq<<std::endl;
    std::cout<<"Get() )Watt "<<drw.Get()<<"\t"<<drw.Get().SiUnit()<<std::endl;
    std::cout<<"Get Consumption "<<c.Get()<<std::endl;
    //~ int x = (drwq / subD);
    std::cout<<"Calc Consumption "<<(drwq / subD)<<std::endl;
    
    auto dd = rw3.Date() - rw2.Date();
    std::cout<<"Days "<<dd<<std::endl;
	auto drw2 = (rw3 -rw2) / dd;
	auto c2 = drw2 / e;
    //~ std::cout<<"Watt "<<drw<<std::endl;
    //~ std::cout<<"Watt "<<drw2.Get()<<std::endl;
    //~ std::cout<<"Cons "<<c2.Get()<<std::endl;
    
    //~ decltype(drw2.Get()) d = 3;
    auto cc = c * c;
    //~ std::cout<<"Cons "<<cc.Get()<<std::endl;
    
	
    return 0;
}
