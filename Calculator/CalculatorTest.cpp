#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "Operations.hpp"
#include "../Home/Counter.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/ShortNames.hpp"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Home/Stage.hpp"
#include "../Quantity/Quantity.hpp"
using namespace ShortNames;
int main()
{       
    std::cout<<"START"<<std::endl;

	auto e3 = Quantity<Sum>{3};
	auto e6 = Quantity<Sum>{6};
	auto e9 = Quantity<Sum>{9};
	auto s2 = Quantity<Scalar>{2};
	auto t1 = Quantity<Time>{3};
	auto t2 = Quantity<Time>{2};
	
	auto a18 = Addition::Calculate(e6,e3)+Addition::Calculate(e6,e3);//+Addition::Calculate(e6,e9);
	auto a18_2 = Addition::Calculate(e9,e9);//+Addition::Calculate(e6,e9);
	
	auto ra1 = a18.Get();
	auto ra12 = a18_2.Get();
	
	std::cout<<a18<<std::endl;
	std::cout<<ra1<<std::endl;
	bool result = std::is_same<decltype(ra1),Q>::value;
	assert(result);
	result = std::is_same<decltype(ra1),decltype(ra12)>::value;
	assert(result);
	assert(ra1.Value()==ra12.Value());

	auto a36 = Addition::Calculate(e6,e3)+Addition::Calculate(e6,e3)+Addition::Calculate(e6,e3)+Addition::Calculate(e6,e3);
	auto a36_2 = Addition::Calculate(e9,e9)+Addition::Calculate(e9,e9);
	
	auto ra2 = a36.Get();
	auto ra2_2 = a36_2.Get();
	
	std::cout<<a36<<std::endl;
	std::cout<<ra2<<std::endl;
	result = std::is_same<decltype(ra2),Q>::value;
	assert(result);
	result = std::is_same<decltype(ra2),decltype(ra2_2)>::value;
	assert(result);
	assert(ra2.Value()==ra2_2.Value());
	//~ auto m2 = Multiplication::Calculate(e3,s2) + Multiplication::Calculate(e3,s2) +Multiplication::Calculate(e3,s2);
	//~ auto d = Division::Calculate(e3,s2);
	//~ std::cout<<Addition::Calculate(e6,e3)<<std::endl;
	//~ std::cout<<Subtraction::Calculate(e6,e3)<<std::endl;
	//~ std::cout<<Multiplication::Calculate(e6,s2)<<std::endl;
	//~ std::cout<<Multiplication::Calculate(e6,e3)<<std::endl;
	//~ std::cout<<Division::Calculate(e6,e3)<<std::endl;
	//~ std::cout<<Division::Calculate(e3,s2)<<std::endl;
	//~ std::cout<<QuantityFraction::Calculate(e6,e3,e9)<<std::endl;
	//~ std::cout<<QuantityFraction::Calculate(e9,e3,e9)<<std::endl;
	//~ std::cout<<QuantityFraction::Calculate(e9,s2,e9)<<std::endl;
	
	//~ std::cout<<"\n---------------------Addition---------------------"<<std::endl;
	//~ std::cout<<(e6+a1+e6)<<std::endl;
	//~ std::cout<<(e6+Addition::Calculate(a1,a1)+e6+Addition::Calculate(a1,m2)+a1)<<std::endl;
	//~ std::cout<<"\n---------------------Subtraction---------------------"<<std::endl;
	//~ std::cout<<(e6-Subtraction::Calculate(a1,a1)-e6-Subtraction::Calculate(a1,m2)+a1)<<std::endl;
	//~ std::cout<<(a1-(e6-a1)-e6)<<std::endl;
	//~ std::cout<<"\n---------------------Multplication---------------------"<<std::endl;
	//~ std::cout<<(e6/Multiplication::Calculate(a1,m2)*Multiplication::Calculate(a1,e6)*m2)<<std::endl;
	//~ std::cout<<"\n---------------------Division---------------------"<<std::endl;
	//~ std::cout<<(e6/Division::Calculate(d,d)/Multiplication::Calculate(a1,e6)/m2)<<std::endl;
	
	//~ Quantity<Time,Days,uint> twoDays = Quantity<Time,Days,uint>(2);
	//~ constexpr auto km86400 = Quantity<Length,Kilo>(86400);
    
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n---------------------Assert---------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;

	//~ auto pE3E6 = Addition::Calculate(e6,e3);
    //~ std::cout<<e3<<" + "<<e6<<" = "<<pE3E6<<std::endl;
    //~ assert(pE3E6.Get().Value()==9);
    //~ bool isSame = std::is_same_v<decltype(pE3E6.Get())::UnitType,Sum>;
    //~ assert(isSame);

	//~ auto sE9E3 = pE3E6 - e3;
    //~ std::cout<<pE3E6<<" - "<<e3<<" = "<<sE9E3<<std::endl;
    //~ assert(sE9E3.Get().Value()==6);
    //~ isSame = std::is_same_v<decltype(sE9E3.Get())::UnitType,Sum>;
    //~ assert(isSame);
	
	//~ auto mE6T2 = sE9E3 * t2;
    //~ std::cout<<sE9E3<<" - "<<t2<<" = "<<mE6T2<<std::endl;
    //~ assert(mE6T2.Get().Value()==12);
    //~ isSame = std::is_same_v<decltype(mE6T2.Get())::UnitType,Unit<1, 0, 0, 1, 0, 0, 0, 0>>;
    //~ assert(isSame);
    
	//~ auto dE12T3 = mE6T2 / t1;
    //~ std::cout<<mE6T2<<" - "<<t1<<" = "<<dE12T3<<std::endl;
    //~ assert(dE12T3.Get().Value()==4);
    //~ isSame = std::is_same_v<decltype(dE12T3.Get())::UnitType,Sum>;
    //~ assert(isSame);
	
	
	std::cout<<"END"<<std::endl;
    return 0;
}
