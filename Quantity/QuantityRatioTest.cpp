#include <iostream>
#include <cassert>
#include "QuantityRatio.hpp"
#include "Quantity.hpp"
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
	
	bool isSame = IsSameTemplate<Minutes::RatioType,Minutes::RatioType>::value;
    assert(isSame);
	isSame = IsSameTemplate<MinutesBase<2>::RatioType,Minutes::RatioType>::value;
    assert(isSame);
    isSame = IsSameTemplate<Minutes::RatioType,Pure::RatioType>::value;
    assert(!isSame);

	constexpr bool isSameC = IsSameTemplate<Minutes::RatioType,Minutes::RatioType>::value;
	
	if constexpr (IsSameTemplate<MinutesBase<2>::RatioType,Minutes::RatioType>::value)
	{}
	
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
    
    
	constexpr auto m1000 = Quantity<Length,Pure>(1000);
	constexpr auto km24 = Quantity<Length,Kilo>(24);
	constexpr auto h24 = Quantity<Time,Hours>(24);
	constexpr auto h1 = Quantity<Time,Hours>(1);
	constexpr auto d1 = Quantity<Time,Days>(1);
	
    assert(d1.PureValue()==86400);
    assert(h1.PureValue()==3600);
    
    auto mPerD = m1000 / d1;
    auto mPerDCastD = QuantityRatio<Days>::Cast(mPerD);
    std::cout<<mPerD<<" Cast as Day "<<mPerDCastD<<std::endl;
    assert(mPerDCastD.Value()==1000);
    
    auto mPerDCastK = QuantityRatio<Kilo>::Cast(mPerD);
    std::cout<<mPerD<<" Cast as Kilo "<<mPerDCastK<<std::endl;
    //~ assert(mPerDCastK.Value()==11.5741);
    
    auto mPerDCastM = QuantityRatio<Mega>::Cast(mPerD);
    std::cout<<mPerD<<" Cast as Mili "<<mPerDCastM<<std::endl;
    //~ assert(mPerDCastM.Value()==11574.1);
    
    auto kmPerh = km24 / h24;
    auto kmPerHCastH = QuantityRatio<Hours>::Cast(kmPerh);
    std::cout<<kmPerh<<" Cast as Mili "<<kmPerHCastH<<std::endl;
    assert(kmPerHCastH.Value()==1);

    
    std::cout<<"END"<<std::endl;

    return 0;
}
