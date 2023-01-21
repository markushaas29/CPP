#include <iostream>
#include "../Reading.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Unit/Unit.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	using RW = Reading<Work>;
	using W = RW::QuantityType;
	
	auto rw = RW{W{123},DateTimes::Date{1,1,2000}};
    std::cout<<"Watt "<<rw<<std::endl;
	
    return 0;
}
