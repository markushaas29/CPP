#include <iostream>
#include <cassert>
#include "QuantityRatio.hpp"
#include "Quantity.hpp"
#include "../CSV/Element.hpp"
#include "../Unit/Unit.hpp"

class QRTest
{
	public:
int Run()
{       
    std::cout<<"QR START"<<std::endl;

	using K = Kilo;
    std::cout<<"L: "<<K::Num<<" / "<<K::Denom<<std::endl;
    assert(K::Num==1000);
    assert(K::Denom==1);
    assert(K::Exponent==1);
    assert(K::Out()=="k");
    
	using K2 = Kilo::PowBy<2>::Type;
    std::cout<<"L: "<<K2::Num<<" / "<<K2::Denom<<std::endl;
    assert(K2::Num==1000000);
    assert(K2::Denom==1);
    assert(K2::Exponent==2);
    assert(K2::Out()=="k");

	using M = Milli;
    std::cout<<"M: "<<M::Num<<" / "<<M::Denom<<std::endl;
    assert(M::BaseNum==1);
    assert(M::BaseDenom==1000);
    assert(M::Exponent==1);
    assert(M::Out()=="m");
	
	using D = Dezi;
    std::cout<<"D: "<<D::Num<<" / "<<D::Denom<<std::endl;
    assert(D::BaseNum==1);
    assert(D::BaseDenom==10);
    assert(D::Exponent==1);
    assert(D::Out()=="d");
	
	using C = Centi;
    assert(C::BaseNum==1);
    assert(C::BaseDenom==100);
    assert(C::Exponent==1);
    assert(C::Out()=="c");
	
	using M2 = Milli::PowBy<2>::Type;
    std::cout<<"M2: "<<M2::Num<<" / "<<M2::Denom<<std::endl;
    assert(M2::BaseNum==1);
    assert(M2::BaseDenom==1000);
    assert(M2::Exponent==2);
    assert(M2::Out()=="m");
	
    std::cout<<"Pure: "<<Pure::Num<<" / "<<Pure::Denom<<std::endl;
    assert(Pure::BaseNum==1);
    assert(Pure::BaseDenom==1);
    assert(Pure::Exponent==1);
    assert(Pure::Out()=="");
	
    std::cout<<"Minutes: "<<Minutes::Num<<" / "<<Minutes::Out()<<std::endl;
    assert(Minutes::BaseNum==60);
    assert(Minutes::BaseDenom==1);
    assert(Minutes::Exponent==1);
    assert(Minutes::Out()=="mins^-1");
    assert(Hours::Out()=="hs^-1");
    
	std::cout<<"Inch: "<<Inch::Num<<" / "<<Inch::Out()<<std::endl;
    assert(Inch::BaseNum==254);
    assert(Inch::BaseDenom==10000);
    assert(Inch::Exponent==1);
    assert(Inch::Out()=="\"");
	
	std::cout<<"Liter: "<<Liter::Num<<" / "<<Liter::Out()<<std::endl;
    assert(Liter::BaseNum==1);
    assert(Liter::BaseDenom==1000);
    assert(Liter::Exponent==1);
    assert(Liter::Out()=="lm^-3");

    std::cout<<"K/h: "<<Minutes::Out()<<std::endl;
    std::cout<<"K/h: "<<Hours::Out()<<std::endl;
	bool same = std::is_same_v<Scalar,QRDiv<Minutes,Hours>::Unit>;
    assert(same);
    std::cout<<"min/H: "<<QRDiv<Minutes,Hours>::Out()<<std::endl;
    assert((QRDiv<Minutes,Hours>::Out()=="minh^-1"));

	same = std::is_same_v<Unit<0,0,0,-2>,QRMul<Minutes,Hours>::Unit>;
    assert(same);
    std::cout<<"min/H: "<<QRMul<Minutes,Hours>::Out()<<std::endl;
    assert((QRMul<Minutes,Hours>::Out()=="minhs^-2"));
    //assert((QRMul<K,Hours>::Factor==(1/3.6)));
	
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
    
    
	auto m1000 = Quantity<Length,Pure, double>(1000);
	auto mm1000 = Quantity<Length,Milli, double>(1);
	auto im = Quantity<Length,Inch>(1);
	auto im5 = Quantity<Length,Inch>(5);
	auto lm = Quantity<Length,Liter>(1);
	auto km24 = Quantity<Length,Kilo>(24);
	auto h24 = Quantity<Time,Hours>(24);
	auto h1 = Quantity<Time,Hours>(1);
	auto d1 = Quantity<Time,Days>(1);
	auto d365 = Quantity<Time,Days>(365);
	auto v365 = Quantity<Volume>(365);
	auto kwh = Quantity<Energy,KiloHour>(1);
	auto kw2 = Quantity<Energy,Kilo>(2);
	
    std::cout<<"Inch "<<im.PureValue()<<std::endl;
    assert(im.Value()==1);
    assert(im.PureValue()==0.0254);
    assert(im5.Value()==5);
    assert(im5.PureValue()==0.127);
    Liter::TransformUnit<Volume>();
	std::cout<<"Liter "<<lm.Out()<<Liter::TransformUnit<Volume>()<<std::endl;
    assert(lm.Value()==1);
    assert(lm.PureValue()==0.001);
	assert(Liter::TransformUnit<Volume>()=="l");

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

    std::cout<<kmPerh<<" Cast as Mili "<<mm1000<<std::endl;
    std::cout<<kwh<<" kwh "<<kwh.PureValue()<<std::endl;
	assert(kwh.PureValue()==3600);

    std::cout<<"KM: \n\t"<<mPerD<<"\n\t"<<mPerD.In<Hours>()<<"\n"<<std::endl;
    std::cout<<"KM: \n\t"<<h1<<"\nkm\t"<<(km24/h1).Data()<<"\n"<<std::endl;
	assert(km24.In<Pure>()==24000);
	assert(m1000.In<Kilo>()==1);
	assert(m1000.In<Mega>()==0.001);
	assert(h1.In<Minutes>()==60);
	assert((km24/h1).Data()=="0.006667kms^-1");

	auto km24Div24h = km24 / h24;
	auto v365Divd365 = v365 / d365; 
	auto kwh48 = h24 * kw2;
    std::cout<<"Day: \n\t"<<km24Div24h<<"\n"<<std::endl;
    std::cout<<"Day: \n\t"<<v365Divd365<<"\n"<<std::endl;
    std::cout<<"KWH: \n\t"<<kwh48<<"\n"<<std::endl;

	assert(km24Div24h<0.00028);
	assert(v365Divd365<0.0001);
    std::cout<<"Day: \n\t"<<h24<<"\nHour\t"<<(Quantity<Scalar>{1}/h1.Transform<>()).In<Hours>()<<"\n"<<std::endl;
	//assert(h1.In<Days>().Equals(0.0416667));
	//assert(h1.In<S>()==0.0416667);
    
    std::cout<<"QR END"<<std::endl;

    return 0;
}
};
