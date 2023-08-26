#include <iostream>
#include <cassert>
#include "Quantity.hpp"
#include "ToQuantity.hpp"
#include "../CSV/Element.hpp"

class StringTest
{
	public:
int Run()
{       
    std::cout<<"\n\tSTART StringTest"<<std::endl;

	auto km1_7 = Quantity<Length,Kilo>("1.7");
	auto km0 = Quantity<Length,Kilo>("");
	auto km177 = Quantity<Length,Kilo>("1,77");
	auto km1000_77 = Quantity<Length,Kilo>("1.000,77");
    std::cout<<"km1,7: "<<km1_7<<std::endl;
    std::cout<<"km1,7: "<<km177<<std::endl;
    std::cout<<"km1,7: "<<km1000_77<<std::endl;
    assert(km1_7.PureValue()==1700);
    assert(km1_7.Value()==1.7);
    assert(km1000_77.Value()==1000.77);
	
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
    std::cout<<"END"<<std::endl;

    return 0;
}
};
