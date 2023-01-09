#include <iostream>
#include <cassert>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;

	using K = Kilo;
    std::cout<<"L: "<<K::Num<<" / "<<K::Denom<<std::endl;
    assert(K::Num==1000);
    assert(K::Denom==1);
    assert(K::Exponent==1);
    
	using K2 = Kilo::PowBy<2>::Type;
    std::cout<<"L: "<<K2::Num<<" / "<<K2::Denom<<std::endl;
    assert(K2::Num==1000000);
    assert(K2::Denom==1);
    assert(K2::Exponent==2);

	using M = Milli;
    std::cout<<"M: "<<M::Num<<" / "<<M::Denom<<std::endl;
    assert(M::BaseNum==1);
    assert(M::BaseDenom==1000);
    assert(M::Exponent==1);
	
	using M2 = Milli::PowBy<2>::Type;
    std::cout<<"M2: "<<M2::Num<<" / "<<M2::Denom<<std::endl;
    assert(M2::BaseNum==1);
    assert(M2::BaseDenom==1000);
    assert(M2::Exponent==2);
	
    std::cout<<"Pure: "<<Pure::Num<<" / "<<Pure::Denom<<std::endl;
    assert(Pure::BaseNum==1);
    assert(Pure::BaseDenom==1);
    assert(Pure::Exponent==1);
	
    std::cout<<"Minutes: "<<Minutes::Num<<" / "<<Minutes::Denom<<std::endl;
    assert(Minutes::BaseNum==60);
    assert(Minutes::BaseDenom==1);
    assert(Minutes::Exponent==1);
	
	
	//~ using L_2 = QuantityRatio<-1,Length>;
	//~ using T_2 = QuantityRatio<-1,Time>;
    //~ std::cout<<L_2::Name<<"L: "<<L_2::Num<<" / "<<L_2::Denom<<std::endl;
    //~ std::cout<<"T: "<<T_2::Num<<" / "<<T_2::Denom<<std::endl;
    //~ assert(L_2::Denom==1000);
    //~ assert(T_2::Denom==60);
    
    //~ using M2 = Minutes<2>;
    //~ std::cout<<"M2: "<<M2::Num<<" / "<<M2::Denom<<std::endl;
    //~ std::cout<<"M2: "<<M2::Name<<std::endl;
    //~ assert(M2::Num==3600);
    //~ assert(M2::Denom==1);
    //~ assert(M2::Exponent==2);
    
    //~ using C = Minutes<1>::Creator<2>;
    //~ std::cout<<"C: "<<C::Num<<" / "<<C::Denom<<std::endl;
    //~ std::cout<<"C: "<<C::Name<<std::endl;
    //~ assert(C::Num==3600);
    //~ assert(C::Denom==1);
    //~ assert(C::Exponent==2);
	
	//~ using PL = Pure<Length>;
    //~ std::cout<<PL::Name<<"L: "<<PL::Num<<" / "<<PL::Denom<<std::endl;
    //~ assert(PL::Denom==1);
    //~ assert(PL::Num==1);

	//~ using KM = Kilo<Length>;
    //~ std::cout<<KM::Name<<"L: "<<KM::Num<<" / "<<KM::Denom<<std::endl;
    //~ assert(KM::Denom==1);
    //~ assert(KM::Num==1000);
    
	using DEKA = Deka;
    std::cout<<DEKA::Name<<"L: "<<DEKA::Num<<" / "<<DEKA::Denom<<std::endl;
    assert(DEKA::Denom==1);
    assert(DEKA::Num==10);
    
    std::cout<<"END"<<std::endl;

    return 0;
}
