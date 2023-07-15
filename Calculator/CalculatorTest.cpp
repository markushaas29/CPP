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
	auto t1 = Quantity<Time>{1};
	auto t2 = Quantity<Time>{2};
	
	auto a18 = Addition::Calculate(e6,e3)+Addition::Calculate(e6,e3);//+Addition::Calculate(e6,e9);
	auto i1 = Addition::Calculate(1,2)+1 -1;//+Addition::Calculate(e6,e9);
	auto a18_2 = Addition::Calculate(e9,e9);//+Addition::Calculate(e6,e9);
	
	auto ra1 = a18.Get();
	auto ra12 = a18_2.Get();
	
	std::cout<<"I: "<<i1<<std::endl;
	std::cout<<ra1<<std::endl;
	bool result = std::is_same<decltype(ra1),Q>::value;
	assert(result);
	result = std::is_same<decltype(ra1),decltype(ra12)>::value;
	assert(result);
	assert(ra1.Value()==ra12.Value());

	auto m18 = Multiplication::Calculate(e3,e3) + Multiplication::Calculate(e3,e3);
	auto rm18 = m18.Get();
	result = std::is_same<decltype(rm18),decltype(e3*e3)>::value;
	assert(result);
	assert(rm18.Value()==ra1.Value());
	
	auto d18 = (m18 / e3)-e3;
	std::cout<<d18<<std::endl;
	auto rd18 = d18.Get();
	result = std::is_same<decltype(rd18),decltype(e3)>::value;
	assert(result);
	assert(rd18==e3);
	
	auto ms6 =  Multiplication::Calculate(e3,s2);
	std::cout<<ms6<<std::endl;
	auto rms6 = ms6.Get();
	result = std::is_same<decltype(rms6),decltype(e3)>::value;
	assert(result);
	assert(rms6==e6);

	auto mst6 =  Multiplication::Calculate(e3,s2) * t1;
	std::cout<<mst6<<std::endl;
	auto rmst6 = mst6.Get();
	result = std::is_same<decltype(rmst6),decltype(e3*t2)>::value;
	assert(result);
	assert(rmst6==(e6*t1));

	auto mstd3 =  mst6 / t2;
	std::cout<<mstd3<<std::endl;
	auto rmstd3 = mstd3.Get();
	rmstd3;
	result = std::is_same<decltype(rmstd3),decltype(e3)>::value;
//	assert(result);
	//~ assert(rmstd3==e3);
	//~ assert(rmstd3.Value()==3);
	
	//~ auto rf3 = Ratio::Calculate(t1,t2,e6);
	//~ //bool x = rf3;
	//~ auto rrf3 = rf3.Get();
	//~ std::cout<<"Ratio\t"<<rf3<<std::endl;
	//~ std::cout<<"Ratio\t"<<rrf3<<std::endl;

	//~ result = std::is_same<decltype(rrf3),decltype(e3)>::value;
	//~ assert(result);
	//~ assert(rrf3==e3);
	//~ assert(rrf3.Value()==3);
//	std::cout<<(e6/Multiplication::Calculate(a1,m2)*Multiplication::Calculate(a1,e6)*m2)<<std::endl;
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
