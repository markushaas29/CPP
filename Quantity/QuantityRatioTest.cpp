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
    assert(K2::Out()=="k^2");

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
    assert(M2::Out()=="m^2");
	
    std::cout<<"Pure: "<<Pure::Num<<" / "<<Pure::Denom<<std::endl;
    assert(Pure::BaseNum==1);
    assert(Pure::BaseDenom==1);
    assert(Pure::Exponent==1);
    assert(Pure::Out()=="");
	
    std::cout<<"Minutes: "<<Minutes::Num<<" / "<<Minutes::Out()<<std::endl;
    assert(Minutes::BaseNum==60);
    assert(Minutes::BaseDenom==1);
    assert(Minutes::Exponent==1);
    assert(Minutes::Out()=="min");
    assert(Hours::Out()=="h");

	auto m1000 = Quantity<Length,Pure, double>(1000);
	auto mm1000 = Quantity<Length,Milli, double>(1000);
	auto h24 = Quantity<Time,Hours>(24);
	auto h1 = Quantity<Time,Hours>(1);
	auto d1 = Quantity<Time,Days>(1);
	auto m1 = Quantity<Time,Minutes>(1);
	auto d365 = Quantity<Time,Days>(365);
	auto v365 = Quantity<Volume>(365);
	auto kwh = Quantity<Energy,KiloHour>(1);
	auto kw2 = Quantity<Energy,Kilo>(2);
	auto km24 = Quantity<Length,Kilo>(24);
	
    std::cout<<"h1*h1: "<<km24.Data()<<std::endl;
    assert(km24.Data()=="24km");
    assert(mm1000.Data()=="1000mm");
    std::cout<<"h1*h1: "<<h24.Data()<<std::endl;
    assert(h24.Data()=="24h");
    assert(h1.Data()=="1h");
    std::cout<<"d1: "<<d1<<std::endl;
    std::cout<<"kw2: "<<kw2<<std::endl;

	auto m2_1000 = m1000 * m1000;
	auto mm2_1000 = mm1000 * mm1000;
    
    assert(m2_1000.Data()=="1000000m^2");
    assert(m2_1000.Value()==1000000);
    assert(m2_1000.PureValue()==1000000);
	assert(decltype(m2_1000)::UnitType::Sign()=="m^2");
	
	std::cout<<"mm10_2: "<<mm2_1000.Data()<<std::endl;
    assert(mm2_1000.Data()=="1000000mm^2");
    assert(mm2_1000.Value()==1000000);
    assert(mm2_1000.PureValue()==1);
	assert(decltype(mm2_1000)::UnitType::Sign()=="m^2");
   
	auto mm10 = Quantity<Length,Milli, double>(10);
	auto mm10_2 = mm10 * mm10;
    
	assert(mm10.Data()=="10mm");
    assert(mm10.Value()==10);
    assert(mm10.PureValue()==0.01);
	assert(decltype(mm10)::UnitType::Sign()=="m");
	assert(decltype(mm10)::QuantityRatioType::Num==1);
	assert(decltype(mm10)::QuantityRatioType::Denom==1000);
    
	std::cout<<"mm10_2: "<<mm10_2.PureValue()<<std::endl;
	std::cout<<"mm10_2: "<<mm10_2.Value()<<std::endl;
	assert(mm10_2.Data()=="100mm^2");
    assert(mm10_2.Value()==100);
    assert(mm10_2.PureValue()<0.00015);
	assert(decltype(mm2_1000)::UnitType::Sign()=="m^2");
	assert(decltype(mm10_2)::QuantityRatioType::Num==1);
	assert(decltype(mm10_2)::QuantityRatioType::Denom==1000000);

	std::cout<<"POW mm10_3: "<<std::endl;
	auto mm10_3 = mm10 * mm10_2;
	std::cout<<"mm10_2: "<<mm10_2<<std::endl;
	std::cout<<"mm10_3: "<<mm10_3<<std::endl;
	std::cout<<"mm10_2Data: "<<mm10_2.Data()<<std::endl;
	std::cout<<"mm10_3Data: "<<mm10_3.Data()<<std::endl;
	std::cout<<"mm10_2: "<<mm10_2.PureValue()<<std::endl;
	std::cout<<"mm10_3: "<<mm10_3.PureValue()<<std::endl;
	std::cout<<"mm10_2: "<<mm10_2.Value()<<std::endl;
	std::cout<<"mm10_3 V: "<<mm10_3.Value()<<std::endl;
	std::cout<<"mm10 Factor: "<<decltype(mm10)::QuantityRatioType::Factor<<std::endl;
	std::cout<<"mm10 Ex: "<<decltype(mm10)::QuantityRatioType::Exponent<<std::endl;
	std::cout<<"mm10_2 Factor: "<<decltype(mm10_2)::QuantityRatioType::Factor<<std::endl;
	std::cout<<"mm10_2 Ex: "<<decltype(mm10_2)::QuantityRatioType::Exponent<<std::endl;
	std::cout<<"mm10_3 Factor: "<<decltype(mm10_3)::QuantityRatioType::Factor<<std::endl;
	std::cout<<"mm10_3 Ex: "<<decltype(mm10_3)::QuantityRatioType::Exponent<<std::endl;
	assert(mm10_3.Data()=="1000mm^3");
    assert(((int)mm10_3.Value())==1000);
    assert(mm10_3.PureValue()<0.0000011);
	assert(decltype(mm10_3)::UnitType::Sign()=="m^3");
	assert(decltype(mm10_3)::QuantityRatioType::Factor==0.000000001);
	assert(decltype(mm10_3)::QuantityRatioType::Num==1);
	assert(decltype(mm10_3)::QuantityRatioType::Denom==1000000000);
	
	auto mm10_4 = mm10 * mm10_3;
	std::cout<<"mm10_4: "<<mm10_4<<std::endl;
	std::cout<<"mm10_3 Ex: "<<decltype(mm10_4)::QuantityRatioType::Factor<<std::endl;
	assert(mm10_4.Data()=="10000mm^4");
    assert(((int)mm10_4.Value())==10000);
    //assert(mm10_4.PureValue()<0.000000002);
	assert(decltype(mm10_4)::UnitType::Sign()=="m^4");
	//assert(decltype(mm10_4)::QuantityRatioType::Factor<0.0000000000002);
	
    assert(mm2_1000.Data()=="1000000mm^2");
    assert(mm2_1000.Value()==1000000);
    assert(mm2_1000.PureValue()==1);

	auto mm3_1000 = mm2_1000 * mm1000;
	std::cout<<"m2_1000: "<<decltype(mm3_1000)::UnitType::Sign()<<std::endl;
	assert(mm3_1000.Data()=="1000000000mm^3");
    assert(mm3_1000.PureValue()==1);
	assert(decltype(mm3_1000)::UnitType::Sign()=="m^3");
	

    assert(m2_1000.Data()=="1000000m^2");
	auto mm_1000_D = mm2_1000 / mm1000;
	auto mm2_1000_D = mm3_1000 / mm1000;
	std::cout<<"mm_1000_D: "<<mm_1000_D.Data()<<std::endl;
	std::cout<<"mm2_1000_D: "<<decltype(mm2_1000_D)::UnitType::Sign()<<std::endl;
    assert(mm_1000_D.Data()=="1000mm");
	assert(decltype(mm_1000_D)::UnitType::Sign()=="m");
	assert(decltype(mm2_1000_D)::UnitType::Sign()=="m^2");
    //assert(mm2_1000_D.Data()=="1000000mm^2");
    
	auto kmh1 = km24 / h24;
	std::cout<<"1 kmh: "<<kmh1.PureValue()<<std::endl;
    assert(kmh1.Data()=="1khm");
    assert(kmh1.PureValue()<0.28);
	assert(decltype(kmh1)::UnitType::Sign()=="ms^-1");

	auto m1_2 = m1 * m1;
    std::cout<<"m1_2: "<<m1_2<<std::endl;
    std::cout<<"K/h: "<<Hours::Out()<<std::endl;
	//bool same = std::is_same_v<Scalar,QRDiv<Minutes,Hours>::Unit>;
    //assert(same);
//    std::cout<<"min/H: "<<QRDiv<Minutes,Hours>::Out()<<std::endl;
//    assert((QRDiv<Minutes,Hours>::Out()=="minh^-1"));
//
//	same = std::is_same_v<Unit<0,0,0,-2>,QRMul<Minutes,Hours>::Unit>;
//    assert(same);
//    std::cout<<"min*H: "<<QRMul<Minutes,Hours>::Out()<<std::endl;
//    assert((QRMul<Minutes,Hours>::Out()=="minh"));
    //assert((QRMul<K,Hours>::Factor==(1/3.6)));
	
//	bool isSame = IsSameTemplate<Minutes::RatioType,Minutes::RatioType>::value;
//    assert(isSame);
//	isSame = IsSameTemplate<MinutesBase<2>::RatioType,Minutes::RatioType>::value;
//    assert(isSame);
//    isSame = IsSameTemplate<Minutes::RatioType,Pure::RatioType>::value;
//    assert(!isSame);

//	assert(false);

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
    
//	using DEKA = Deka;
//    std::cout<<DEKA::Name<<"L: "<<DEKA::Num<<" / "<<DEKA::Denom<<std::endl;
//    assert(DEKA::Denom==1);
//    assert(DEKA::Num==10);
//    
//    
//    assert(d1.PureValue()==86400);
//    assert(h1.PureValue()==3600);
//    
//    auto mPerD = m1000 / d1;
//    auto mPerDCastD = QuantityRatio<Days>::Cast(mPerD);
//    std::cout<<mPerD<<" Cast as Day "<<mPerDCastD<<std::endl;
//    assert(mPerDCastD.Value()==1000);
//    
//    auto mPerDCastK = QuantityRatio<Kilo>::Cast(mPerD);
//    std::cout<<mPerD<<" Cast as Kilo "<<mPerDCastK<<std::endl;
//    //~ assert(mPerDCastK.Value()==11.5741);
//    
//    auto mPerDCastM = QuantityRatio<Mega>::Cast(mPerD);
//    std::cout<<mPerD<<" Cast as Mili "<<mPerDCastM<<std::endl;
//    //~ assert(mPerDCastM.Value()==11574.1);
//    
//    auto kmPerh = km24 / h24;
//    auto kmPerHCastH = QuantityRatio<Hours>::Cast(kmPerh);
//    std::cout<<kmPerh<<" Cast as Mili "<<kmPerHCastH<<std::endl;
//    assert(kmPerHCastH.Value()==1);
//
//    std::cout<<kmPerh<<" Cast as Mili "<<mm1000<<std::endl;
//    std::cout<<kwh<<" kwh "<<kwh.PureValue()<<std::endl;
//	assert(kwh.PureValue()==3600);
//
//    std::cout<<"KM: \n\t"<<mPerD<<"\n\t"<<mPerD.In<Hours>()<<"\n"<<std::endl;
//    std::cout<<"KM: \n\t"<<h1<<"\nkm\t"<<(km24/h1).Data()<<"\n"<<std::endl;
//	assert(km24.In<Pure>()==24000);
//	assert(m1000.In<Kilo>()==1);
//	assert(m1000.In<Mega>()==0.001);
//	assert(h1.In<Minutes>()==60);
//	assert((km24/h1).Data()=="0.006667kms^-1");
//
//	auto km24Div24h = km24 / h24;
//	auto v365Divd365 = v365 / d365; 
//	auto kwh48 = h24 * kw2;
//    std::cout<<"Day: \n\t"<<km24Div24h<<"\n"<<std::endl;
//    std::cout<<"Day: \n\t"<<v365Divd365<<"\n"<<std::endl;
//    std::cout<<"KWH: \n\t"<<kwh48<<"\n"<<std::endl;
//
//	assert(km24Div24h<0.00028);
//	assert(v365Divd365<0.0001);
//    //std::cout<<"Day: \n\t"<<h24<<"\nHour\t"<<(Quantity<Scalar>{1}/h1.Transform<>()).In<Hours>()<<"\n"<<std::endl;
//	//assert(h1.In<Days>().Equals(0.0416667));
//	//assert(h1.In<S>()==0.0416667);
//    
//    std::cout<<"QR END"<<std::endl;

    return 0;
}
};
