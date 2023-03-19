#include <iostream>
#include <iostream>
#include "../../File/Repository.hpp"
#include "../../Unit/Unit.hpp"
#include "../Reading.hpp"
#include "../House.hpp"
#include "../Counter.hpp"
#include "../CounterConfiguration.hpp"
#include "../CounterContainer.hpp"
#include "../../Common/ShortNames.hpp"
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

	//House<HouseConfiguration>::Instance();
	CSV::Repository<CSV::CounterParsers>::Instance();
	CounterContainerType::Instance().Display(std::cout);
	std::cout<<CounterContainerType::Instance().Get<1>()<<std::endl;
	assert(11144078==CounterContainerType::Instance().Get<1>());
    	return 0;
}
