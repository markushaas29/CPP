#include <iostream>
#include <cassert>
#include "Quantity.hpp"
#include "ToQuantity.hpp"
#include "../CSV/Element.hpp"
#include "../To/To.hpp"

class StringTest
{
	public:
int Run()
{       
    std::cout<<"\n\tSTART StringTest"<<std::endl;

	auto km1_7 = Quantity<Length,Kilo>("1.7 km");
	auto km0 = Quantity<Length,Kilo>("");
	auto km177 = Quantity<Length,Kilo>("1,77€");
	auto km1000_77 = Quantity<Length,Kilo>("1.000,77DM");
	auto km1000_7789 = Quantity<Length,Kilo>("1.000,7789km²");
	auto m1000_7789 = Quantity<Length,Kilo>("1.000,7789m²");
	auto m177 = Quantity<Length,Kilo>("1,77m²");
    std::cout<<"km1,7: "<<km1_7<<std::endl;
    std::cout<<"km1,7: "<<km177<<std::endl;
    std::cout<<"km1,7: "<<km1000_7789<<std::endl;
    assert(km1_7.PureValue()==1700);
    assert(km1_7.Value()==1.7);
    assert(km1000_77.Value()==1000.77);
    assert(km1000_7789.Value()==1000.7789);
	
	auto km17000 = Quantity<Length,Kilo,uint>("17000");
	auto m1_77 = Quantity<Area>(m177);
    assert(m1_77.Value()==1.77);
	auto m1000_77 = Quantity<Area>(m1000_7789);
    assert(m1000_77.Value()==1000.7789);
    std::cout<<"A 1000,789: "<<m1000_77<<std::endl;
	auto km170000 = Quantity<Length,Kilo,int>("170000");
    assert(km170000.Value()==170000);
	
	std::string s123_9("12.9");
	auto q1239 = Quantity<Sum>(s123_9);
	auto qs954_0 = Quantity<Sum>("954,0");
	auto qs9540 = Quantity<Sum>("954.0");
	auto qs1000_3 = Quantity<Sum>("1.000,3");
	auto qs10003 = Quantity<Sum>("1000.3");
	std::cout<<q1239<<std::endl;
	std::cout<<q1239.Value()<<std::endl;
	assert(qs954_0.Value()==954.0);
	assert(qs9540.Value()==954.0);
	assert(qs1000_3.Value()==1000.3);
	assert(qs10003.Value()==1000.3);
	
	auto qs123_456 = Quantity<Sum>("123,456");
	assert(qs123_456.Value()==123.456);
	auto qs123456 = Quantity<Sum>("123.456");
	assert(qs123456.Value()==123.456);
	
	auto qs2_123_456 = Quantity<Sum>("2.123,456");
	assert(qs2_123_456.Value()==2123.456);
	auto qs123456789 = Quantity<Sum>("123.456789");
	assert(qs123456789.Value()==123.456789);
	
	auto qs2_789_123_456 = Quantity<Sum>("2.789.123,456");
	assert(qs2_789_123_456.Value()==2789123.456);
	auto qs2_123_456789 = Quantity<Sum>("2.123,456789");
	assert(qs2_123_456789.Value()==2123.456789);
	auto qs2123456789 = Quantity<Sum>("2123,456789");
	assert(qs2123456789.Value()==2123.456789);

	auto qs123_5 = To<Quantity<Sum>>("123.5");
	assert(qs123_5.Value()==123.5);

    std::cout<<"END"<<std::endl;

    return 0;
}
};
