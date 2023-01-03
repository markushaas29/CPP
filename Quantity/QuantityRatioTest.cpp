#include <iostream>
#include <cassert>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;

	using L = QuantityRatio<1,Length>;
	using T = QuantityRatio<1,Time>;
    std::cout<<"L: "<<L::Num<<" / "<<L::Denom<<std::endl;
    std::cout<<"T: "<<T::Num<<" / "<<T::Denom<<std::endl;
    assert(L::Num==1000);
    assert(T::Num==60);
	
	using L2 = QuantityRatio<2,Length>;
	using T2 = QuantityRatio<2,Time>;
    std::cout<<"L: "<<L2::Num<<" / "<<L2::Denom<<std::endl;
    std::cout<<"T: "<<T2::Num<<" / "<<T2::Denom<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
