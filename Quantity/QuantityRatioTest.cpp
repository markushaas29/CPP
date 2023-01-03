#include <iostream>
#include <cassert>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;

	using L = QuantityRatio<1,Length>;
	using T = QuantityRatio<1,Time>;
    assert(L::Num==1000);
    assert(T::Num==60);
	
    std::cout<<"END"<<std::endl;

    return 0;
}
