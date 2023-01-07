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
	
	auto km1_7 = Quantity<Length,Kilo<Length>>("1,7");
    std::cout<<"km1,7: "<<km1_7<<std::endl;
    assert(km1_7.PureValue()==1700);
    assert(km1_7.Value()==1.7);
	
	Quantity<Length,Kilo<Length>>::UnitType u;
	auto km1 = Quantity<Length,Kilo<Length>>(1);
	auto km2 = Quantity<Length,Kilo<Length>>(2);
	auto m500 = Quantity<Length,Pure<Length>>(500);
	
    std::cout<<"km1: "<<km1<<std::endl;
    std::cout<<"m500: "<<m500<<std::endl;
    assert(km1.PureValue()==1000);
    assert(km1.Value()==1);
    assert(decltype(km1)::QuantityRatioType::Exponent==1);
	
	auto km15 = km1 + m500;
    std::cout<<"km15: "<<km15<<std::endl;
    assert(km15.PureValue()==1500);
    assert(km15.Value()==1.5);
	
	auto m1500 = m500 + km1;
    std::cout<<"m1500: "<<m1500<<std::endl;
    assert(m1500.PureValue()==1500);
    assert(m1500.Value()==1500);
	
	auto km15m500 = km15 - m500;
    std::cout<<"km15 minus: "<<km15m500<<std::endl;
    assert(km15m500.PureValue()==1000);
    assert(km15m500.Value()==1);
	
	auto m1500m = m1500 - km1;
    std::cout<<"m1500m: "<<m1500m<<std::endl;
    assert(m1500m.PureValue()==500);
    assert(m1500m.Value()==500);
	
	auto km12 = km1 * km2;
    std::cout<<"km12: "<<km12<<std::endl;
    assert(km12.PureValue()==2000000);
    assert(km12.Value()==2);
    assert(decltype(km12)::QuantityRatioType::Exponent==2);
	
	auto km1p2 = km1 + km2;
    std::cout<<"km12: "<<km1p2<<std::endl;
    assert(km1p2.PureValue()==3000);
    assert(km1p2.Value()==3);
    assert(decltype(km1p2)::QuantityRatioType::Exponent==1);
	
	auto km2m1 = km2 - km1;
    std::cout<<"km2m1: "<<km2m1<<std::endl;
    assert(km2m1.PureValue()==1000);
    assert(km2m1.Value()==1);
    assert(decltype(km2m1)::QuantityRatioType::Exponent==1);
	
	//~ auto kmP = km1 + km2;
    //~ std::cout<<"km: Pure "<<kmP.Value()<<std::endl;
    //~ std::cout<<"km: Value() "<<kmP.PureValue()<<std::endl;
	
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
	
	//~ Quantity<Time,Days> d3 = Quantity<Time,Days>(1);
    //~ std::cout<<"Days: "<<d3<<std::endl;
    //~ std::cout<<"Days: "<<d3<<std::endl;
    //~ std::cout<<"Days: "<<d3.ToPure()<<std::endl;
    //~ assert(d3.PureValue()==86400);
    
    //~ auto d2 = d3 + d3;
	//~ Quantity<Time,Hours> h1 = Quantity<Time,Hours>(1);
	auto m1 = Quantity<Time,Minutes<>>(1);
	auto m2 = Quantity<Time,Minutes<>>(2);
    //~ std::cout<<"H: "<<h1<<std::endl;
    //~ std::cout<<"M: "<<m1<<std::endl;
    //~ assert(d2.PureValue()==(2 * 86400));
    //~ assert(h1.PureValue()==3600);
    //~ assert(m1.PureValue()==60);
	
	//~ auto dhm = d3 + h1 + m1;
    //~ std::cout<<"dhm: "<<dhm<<dhm.PureValue()<<std::endl;
    //~ assert(dhm.PureValue()==(86400+3600+60));
	//~ dhm = dhm - m1;
    //~ assert(dhm.PureValue()==(86400+3600));
	
	auto m11 = m1 * m1;
    std::cout<<"m11: "<<m11<<"\t"<<m11.PureValue()<<std::endl;
    std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Num<<std::endl;
    std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Name<<std::endl;
    std::cout<<"m11: "<<decltype(m11)::QuantityRatioType::Name<<std::endl;
    assert(m11.PureValue()==3600);
    assert(m11.Value()==1);
    assert(decltype(m11)::QuantityRatioType::Exponent==2);
	
	auto m111 = m11 * m1;
    std::cout<<"m11: "<<m111<<"\t"<<m111.PureValue()<<std::endl;
    std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Num<<std::endl;
    std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Name<<std::endl;
    std::cout<<"m111: "<<decltype(m111)::QuantityRatioType::Name<<std::endl;
    assert(decltype(m111)::QuantityRatioType::Exponent==3);
    assert(m111.PureValue()==216000);
    assert(m111.Value()==1);
	
	auto m22 = m2 * m2;
    std::cout<<"m11: "<<m22<<"\t"<<m22.PureValue()<<std::endl;
    assert(m22.PureValue()==14400);
    assert(m22.Value()==4);
	
    std::cout<<"END"<<std::endl;

    return 0;
}
