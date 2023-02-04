#include <iostream>
#include <cassert>
#include "Quantity.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;

	
	//~ Quantity<Mass> q5 = Quantity<Mass>(3);
	//~ Quantity<Mass> q7 = Quantity<Mass>(13);
	//~ Quantity<Unit<0,0,2>> q8 = Quantity<Unit<0,0,2>>(13);
	//~ using qT = Quantity<Unit<0,0,2>>::Type;
	
	//~ auto q6 = q7 +q5;
	//~ std::cout<<"q6: "<<q6.Value()<<std::endl;
	
	//~ q6 = q7 - q5;
	//~ std::cout<<"q6: "<<q6.Value()<<std::endl;
	
	//~ auto q9 = q8 * q5;
    //~ std::cout<<"q9: "<<q9.Value()<<std::endl;
	
	//~ auto q11 = q8 / q5;
    //~ std::cout<<"q11: "<<q11.Value()<<std::endl;
	
	//~ auto q10 = q7 * q5;
    //~ std::cout<<"q10: "<<q9.Value()<<std::endl;
	
	//~ auto q12 = q7 / q5;
    //~ std::cout<<"q12: "<<q12.Value()<<std::endl;
	
	auto km1_7 = Quantity<Length,Kilo>("1,7");
    std::cout<<"km1,7: "<<km1_7<<std::endl;
    assert(km1_7.PureValue()==1700);
    assert(km1_7.Value()==1.7);
	
	Quantity<Length,Kilo>::UnitType u;
	constexpr auto km1 = Quantity<Length,Kilo>(1);
	constexpr auto km2 = Quantity<Length,Kilo>(2);
	constexpr auto m500 = Quantity<Length,Pure>(500);
	constexpr auto m1000 = Quantity<Length,Pure>(1000);
	constexpr auto m2000 = Quantity<Length,Pure>(2000);
	constexpr auto mm500 = Quantity<Length,Milli>(500);
	
	constexpr auto g500 = Quantity<Mass,Pure>(500);
	constexpr auto kg1 = Quantity<Mass,Kilo>(1);
	constexpr auto mg1000 = Quantity<Mass,Milli>(1000);
    std::cout<<"km1: "<<km1<<std::endl;
    std::cout<<"m500: "<<m500<<std::endl;
    assert(km1.PureValue()==1000);
    assert(km1.Value()==1);
    assert(decltype(km1)::QuantityRatioType::Exponent==1);
	
	constexpr auto km15 = km1 + m500;
    std::cout<<km1<<" + "<<m500<<" = "<<km15<<std::endl;
    std::cout<<"km15: "<<km15<<std::endl;
    std::cout<<"km15Pure: "<<km15.PureValue()<<std::endl;
    assert(km15.PureValue()==1500);
    assert(km15.Value()==1.5);
	
	constexpr auto m1500 = m500 + km1;
    std::cout<<"m1500: "<<m1500<<std::endl;
    assert(m1500.PureValue()==1500);
    assert(m1500.Value()==1500);
	
	constexpr auto mm1500 = mm500 + m500 + km1;
    std::cout<<"mm1500: "<<mm1500<<std::endl;
    assert(m1500.PureValue()==1500);
    assert(m1500.Value()==1500);
	
	constexpr auto km15m500 = km15 - m500;
    std::cout<<"km15 minus: "<<km15m500<<std::endl;
    assert(km15m500.PureValue()==1000);
    assert(km15m500.Value()==1);
	
	constexpr auto m1500m = m1500 - km1;
    std::cout<<"m1500m: "<<m1500m<<std::endl;
    assert(m1500m.PureValue()==500);
    assert(m1500m.Value()==500);
	
	constexpr auto qkm1 = km1 * m1000;
    std::cout<<"qkm1: "<<qkm1<<std::endl;
    assert(qkm1.Value()==1);
    assert(qkm1.PureValue()==1000000);
	
	constexpr auto km12 = km1 * km2;
    std::cout<<"km12: "<<km12<<std::endl;
    assert(km12.PureValue()==2000000);
    assert(km12.Value()==2);
    assert(decltype(km12)::QuantityRatioType::Exponent==2);
    static_assert(std::is_same_v<decltype(km12)::QuantityRatioType,KiloBase<2>>,"Data structure requires default-constructible elements");
	
	constexpr auto s1 = km2 / km2;
    std::cout<<km2<<" / "<<km2<<" = "<<s1<<std::endl;
    assert(s1.PureValue()==1);
    assert(s1.Value()==1);
    assert(decltype(s1)::QuantityRatioType::Exponent==1);
	
	constexpr auto sm1 = km2 / m1000;
    std::cout<<km2<<" / "<<m1000<<" = "<<sm1<<std::endl;
    assert(sm1.PureValue()==2);
    assert(sm1.Value()==2);
    assert(decltype(sm1)::QuantityRatioType::Exponent==1);
	
	constexpr auto km1d = km12 / km2;
    std::cout<<km12<<" / "<<km2<<" = "<<km1d<<std::endl;
    assert(km1d.PureValue()==1000);
    assert(km1d.Value()==1);
    assert(decltype(km1d)::QuantityRatioType::Exponent==1);

	constexpr auto kmd1m = km12 / m2000;
    std::cout<<km12<<" / "<<m2000<<" = "<<kmd1m<<std::endl;
    assert(kmd1m.PureValue()==1000);
    assert(kmd1m.Value()==1);
    assert(decltype(kmd1m)::QuantityRatioType::Exponent==1);

	constexpr auto kg1dkm1 = kg1 / km1;
    std::cout<<kg1<<" / "<<km1<<" = "<<kg1dkm1<<std::endl;
    assert(kg1dkm1.PureValue()==1);
    assert(kg1dkm1.Value()==1);
    assert(decltype(kg1dkm1)::QuantityRatioType::Exponent==0);
	
	constexpr auto kg1mkm1 = kg1 * km1;
    std::cout<<kg1<<" * "<<km1<<" = "<<kg1mkm1<<std::endl;
    assert(kg1mkm1.PureValue()==1000000);
    assert(kg1mkm1.Value()==1);
    assert(decltype(kg1mkm1)::QuantityRatioType::Exponent==2);
	
	constexpr auto km1p2 = km1 + km2;
    std::cout<<"km12: "<<km1p2<<std::endl;
    assert(km1p2.PureValue()==3000);
    assert(km1p2.Value()==3);
    assert(decltype(km1p2)::QuantityRatioType::Exponent==1);
	
	constexpr auto km2m1 = km2 - km1;
    std::cout<<"km2m1: "<<km2m1<<std::endl;
    assert(km2m1.PureValue()==1000);
    assert(km2m1.Value()==1);
    assert(decltype(km2m1)::QuantityRatioType::Exponent==1);
	
	constexpr auto be = km2 == km1;
	constexpr auto bue = km2 != km1;
	constexpr auto bg = km2 > km1;
	constexpr auto bl = km2 < km1;
	auto kmP = km1 + km2;
    std::cout<<"km: Pure "<<kmP.Value()<<std::endl;
    std::cout<<"km: Value() "<<kmP.PureValue()<<std::endl;
	
    //~ std::cout<<"q5: Pure "<<q5.Value()<<std::endl;
    //~ std::cout<<"q5: Value() "<<q5.PureValue()<<std::endl;
    //~ std::cout<<"km1: Pure "<<km1.Value()<<std::endl;
    //~ std::cout<<"km1: Value() "<<km1.PureValue()<<std::endl;
	
	//~ auto kmP2 = km1 + q5;
    //~ std::cout<<"km: Pure "<<kmP2.Value()<<std::endl;
    //~ std::cout<<"km: Value() "<<kmP2.PureValue()<<std::endl;
		
	//~ auto kmMi2 = km1 - q5;
    //~ std::cout<<"km: Pure "<<kmMi2.Value()<<std::endl;
    //~ std::cout<<"km: Value() "<<kmMi2.PureValue()<<std::endl;
		
	//~ auto kmM = km1 * km2;
    //~ std::cout<<"km1 * km2: Pure "<<kmM.Value()<<std::endl;
    //~ std::cout<<"km1 * km2: Value() "<<kmM.PureValue()<<std::endl;
	
	//~ auto kmM2 = km1 * q5;
    //~ std::cout<<"km1 * q5: Pure "<<kmM2.Value()<<std::endl;
    //~ std::cout<<"km1 * q5: Value() "<<kmM2.PureValue()<<std::endl;
	
	//~ auto kmD = kmM2 / q5;
    //~ std::cout<<"kmM2 / q5: Pure "<<kmD.Value()<<std::endl;
    //~ std::cout<<"kmM2 / q5: Value() "<<kmD.PureValue()<<std::endl;
	
	Quantity<Time,Days> d3 = Quantity<Time,Days>(1);
    std::cout<<"Days: "<<d3<<std::endl;
    std::cout<<"Days: "<<d3<<std::endl;
    std::cout<<"Days: "<<d3.ToPure()<<std::endl;
    assert(d3.PureValue()==86400);
    
    auto d2 = d3 + d3;
	Quantity<Time,Hours> h1 = Quantity<Time,Hours>(1);
	auto m1 = Quantity<Time,Minutes>(1);
	auto m2 = Quantity<Time,Minutes>(2);
    std::cout<<"H: "<<h1<<std::endl;
    std::cout<<"M: "<<m1<<std::endl;
    assert(d2.PureValue()==(2 * 86400));
    assert(h1.PureValue()==3600);
    assert(m1.PureValue()==60);
	
	Quantity<Time,Days,uint> twoDays = Quantity<Time,Days,uint>(2);
	auto kmPerDays = km1 / twoDays;
    std::cout<<"KmPerDays: "<<kmPerDays<<std::endl;
    assert(kmPerDays.Value()==0.5);
    std::cout<<"KmPerDays Pure: "<<kmPerDays.PureValue()<<std::endl;
    //~ assert(kmPerDays.PureValue()==(0.5 * 86400));
    bool isSame = std::is_same_v<decltype(kmPerDays)::UnitType,Unit<0, 1, 0, -1, 0, 0, 0, 0>>;
    assert(isSame);
	
	auto kmProDays = km1 * twoDays;
    std::cout<<"KmPerDays: "<<kmProDays<<std::endl;
    assert(kmProDays.Value()==2);
    std::cout<<"KmPerDays Pure: "<<kmProDays.PureValue()<<std::endl;
    //~ assert(kmProDays.PureValue()==(0.5 * 86400));
    isSame = std::is_same_v<decltype(kmProDays)::UnitType,Unit<0, 1, 0, 1, 0, 0, 0, 0>>;
    assert(isSame);
    
	//~ auto dhm = d3 + h1 + m1;
    //~ std::cout<<"dhm: "<<dhm<<dhm.PureValue()<<std::endl;
    //~ assert(dhm.PureValue()==(86400+3600+60));
	//~ dhm = dhm - m1;
    //~ assert(dhm.PureValue()==(86400+3600));
	
	//~ auto m11 = m1 * m1;
    //~ std::cout<<"m11: "<<m11<<"\t"<<m11.PureValue()<<std::endl;
    //~ std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Num<<std::endl;
    //~ std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Name<<std::endl;
    //~ std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Name<<std::endl;
    //~ assert(m11.PureValue()==3600);
    //~ assert(m11.Value()==1);
    //~ assert(decltype(m11)::QuantityRatioType::Exponent==2);
	
	//~ auto m111 = m11 * m1;
    //~ std::cout<<"m11: "<<m111<<"\t"<<m111.PureValue()<<std::endl;
    //~ std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Num<<std::endl;
    //~ std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Name<<std::endl;
    //~ std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Name<<std::endl;
    //~ assert(decltype(m111)::QuantityRatioType::Exponent==3);
    //~ assert(m111.PureValue()==216000);
    //~ assert(m111.Value()==1);
	
	//~ auto m22 = m2 * m2;
    //~ std::cout<<"m11: "<<m22<<"\t"<<m22.PureValue()<<std::endl;
    //~ assert(m22.PureValue()==14400);
    //~ assert(m22.Value()==4);
	
    std::cout<<"END"<<std::endl;

    return 0;
}
