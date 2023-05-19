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
	auto wp = W{123.456};
	auto w2 = W{124};
	auto w3 = W{148};
	auto e = Q{2};
	
	assert(w3>w2);
	assert(w2<w3);
	auto dt = DT{1,2,2003};
	auto dt2 = DT{1,2,2004};
	auto dt3 = DT{24,2,2004};

	assert(dt3>dt2);
	assert(dt2<dt3);

	auto rw = RW{w,dt};
	auto rw2 = RW{w2,dt2};
	
	auto rw3 = RW{w2,dt2};
	auto rw4 = RW{w2,dt3};
	auto rw5 = RW{w3,dt2};
	auto rw6 = RW{w3,dt3};
	assert(w2!=w3);
	assert(w2<w3);
	assert(w3>w2);
	assert(w2!=w3);
	
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
    	std::cout<<"Watt "<<drw<<std::endl;
   	std::cout<<"Q Watt "<<drwq<<std::endl;
    	std::cout<<"Get() )Watt "<<drw.Get()<<"\t"<<drw.Get().SiUnit()<<std::endl;
    	std::cout<<"Get Consumption "<<c.Get()<<std::endl;
   	std::cout<<"Calc Consumption "<<(drwq / subD)<<std::endl;
    
   	auto dd = rw3.Date() - rw2.Date();
    	std::cout<<"Days "<<dd<<std::endl;
	auto drw2 = (rw3 -rw2) / dd;
	auto c2 = drw2 / e;
    
    	std::cout<<"3 "<<rw3<<rw3.Value()<<std::endl;
    	std::cout<<"4 "<<rw4<<rw4.Value()<<std::endl;
    	std::cout<<"5 "<<rw5<<rw4.Value()<<std::endl;
    	std::cout<<"6 "<<rw6<<rw4.Value()<<std::endl;
	assert(rw3==rw3);
	assert(rw3!=rw5);
	assert(rw5>=rw3);
	assert(rw5>=rw3);
	assert(rw3<=rw6);
	assert(rw3<rw4);
	assert(rw3<rw6);
	assert(rw3<rw4);
    	auto cc = c * c;

	std::istringstream is(std::string("1 2 2003") + " " + std::string("123"));
	auto in = RW::Create(is);
	assert(in.Value()==w);
	assert(in.Date()==dt);
	
	std::istringstream isp2(std::string("1 2 2003") + " " + std::string("123.456"));
	auto inp2 = RW::Create(isp2);
    std::cout<<"inp2 "<<inp2<<std::endl;
	assert(inp2.Value()==wp);
	assert(inp2.Date()==dt);
	std::cout<<inp2<<std::endl;

	std::istringstream isp(std::string("1.2.2003") + " " + std::string("123.456"));
	auto inp = RW::Create(isp);
    std::cout<<"inp "<<inp<<std::endl;
	assert(inp.Value()==wp);
	assert(inp.Date()==dt);
	assert(std::cin);
   	std::cout<<in<<std::endl;
	
    return 0;
}
