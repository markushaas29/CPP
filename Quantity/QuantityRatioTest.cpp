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
    std::cout<<"Minutes: "<<Minutes<1>::Num<<" / "<<Minutes<1>::Denom<<std::endl;
    assert(L2::Num==1000000);
    assert(Minutes<1>::Num==60);
    assert(T2::Num==3600);
	
	using L_2 = QuantityRatio<-1,Length>;
	using T_2 = QuantityRatio<-1,Time>;
    std::cout<<L_2::Name<<"L: "<<L_2::Num<<" / "<<L_2::Denom<<std::endl;
    std::cout<<"T: "<<T_2::Num<<" / "<<T_2::Denom<<std::endl;
    assert(L_2::Denom==1000);
    assert(T_2::Denom==60);
    
    using M2 = Minutes<2>;
    std::cout<<"M2: "<<M2::Num<<" / "<<M2::Denom<<std::endl;
    std::cout<<"M2: "<<M2::Name<<std::endl;
    assert(M2::Num==3600);
    assert(M2::Denom==1);
    assert(M2::Exponent==2);
    
    using C = Minutes<1>::Creator<2>;
    std::cout<<"C: "<<C::Num<<" / "<<C::Denom<<std::endl;
    std::cout<<"C: "<<C::Name<<std::endl;
    assert(C::Num==3600);
    assert(C::Denom==1);
    assert(C::Exponent==2);
	
	using PL = Pure<Length>;
    std::cout<<PL::Name<<"L: "<<PL::Num<<" / "<<PL::Denom<<std::endl;
    assert(PL::Denom==1);
    assert(PL::Num==1);

	using KM = Kilo<Length>;
    std::cout<<KM::Name<<"L: "<<KM::Num<<" / "<<KM::Denom<<std::endl;
    assert(KM::Denom==1);
    assert(KM::Num==1000);
    
    std::cout<<"END"<<std::endl;

    return 0;
}
