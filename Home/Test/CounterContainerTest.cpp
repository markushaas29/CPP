#include <iostream>
#include "../../Common/ShortNames.hpp"
#include "../../Unit/Unit.hpp"
#include "../Reading.hpp"
#include "../Counter.hpp"
#include "../CounterConfiguration.hpp"
#include "../CounterContainer.hpp"
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
	std::cout<<CounterContainerType::Instance();

    return 0;
}
