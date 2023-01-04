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
	
	
	//~ Quantity<Mass,Kilo>::UnitType u;
	//~ auto km1 = Quantity<Mass,Kilo>(2);
	//~ auto km2 = Quantity<Mass,Kilo>(2);
	//~ qT::ValueType v;
	
    //~ std::cout<<"v: "<<v<<std::endl;
	
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
	
	auto m22 = m2 * m2;
    std::cout<<"m11: "<<m22<<"\t"<<m22.PureValue()<<std::endl;
    assert(m22.PureValue()==14400);
    assert(m22.Value()==4);
	
    std::cout<<"END"<<std::endl;

    return 0;
}
