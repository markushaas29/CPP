#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "Operations.hpp"
#include "../Home/Counter.hpp"
#include "../Common/DateTimes.hpp"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Home/Stage.hpp"
#include "../Quantity/Quantity.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;

	auto e3 = Quantity<Sum>{3};
	auto e6 = Quantity<Sum>{6};
	auto e9 = Quantity<Sum>{9};
	auto s2 = Quantity<Scalar>{2};
	auto t1 = Quantity<Time>{3};
	auto t2 = Quantity<Time>{2};
	
	auto a1 = Addition::Calculate(e6,e9)+Addition::Calculate(e6,e9)+Addition::Calculate(e6,e9);
	auto m2 = Multiplication::Calculate(e3,s2) + Multiplication::Calculate(e3,s2) +Multiplication::Calculate(e3,s2);
	auto d = Division::Calculate(e3,s2);
	
	std::cout<<Multiplication::Calculate(3,3)<<std::endl;
	std::cout<<Addition::Calculate(e6,e3)<<std::endl;
	std::cout<<Subtraction::Calculate(e6,e3)<<std::endl;
	std::cout<<Multiplication::Calculate(e6,s2)<<std::endl;
	std::cout<<Multiplication::Calculate(e6,e3)<<std::endl;
	std::cout<<Division::Calculate(e6,e3)<<std::endl;
	std::cout<<Division::Calculate(e3,s2)<<std::endl;
	std::cout<<QuantityRatio::Calculate(e6,e3,e9)<<std::endl;
	std::cout<<QuantityRatio::Calculate(e9,e3,e9)<<std::endl;
	std::cout<<QuantityRatio::Calculate(e9,s2,e9)<<std::endl;
	
	std::cout<<"\n---------------------Addition---------------------"<<std::endl;
	std::cout<<(e6+a1+e6)<<std::endl;
	std::cout<<(e6+Addition::Calculate(a1,a1)+e6+Addition::Calculate(a1,m2)+a1)<<std::endl;
	std::cout<<"\n---------------------Subtraction---------------------"<<std::endl;
	std::cout<<(e6-Subtraction::Calculate(a1,a1)-e6-Subtraction::Calculate(a1,m2)+a1)<<std::endl;
	std::cout<<(a1-(e6-a1)-e6)<<std::endl;
	std::cout<<"\n---------------------Multplication---------------------"<<std::endl;
	std::cout<<(e6/Multiplication::Calculate(a1,m2)*Multiplication::Calculate(a1,e6)*m2)<<std::endl;
	std::cout<<"\n---------------------Division---------------------"<<std::endl;
	std::cout<<(e6/Division::Calculate(d,d)/Multiplication::Calculate(a1,e6)/m2)<<std::endl;
	
	Quantity<Time,Days,uint> twoDays = Quantity<Time,Days,uint>(2);
	constexpr auto km86400 = Quantity<Length,Kilo>(86400);
    
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;
	std::cout<<"\n---------------------Assert---------------------"<<std::endl;
	std::cout<<"\n------------------------------------------"<<std::endl;

	auto pE3E6 = Addition::Calculate(e6,e3);
    std::cout<<e3<<" + "<<e6<<" = "<<pE3E6<<std::endl;
    assert(pE3E6.Get().Value()==9);
    bool isSame = std::is_same_v<decltype(pE3E6.Get())::UnitType,Sum>;
    assert(isSame);

	auto sE9E3 = pE3E6 - e3;
    std::cout<<pE3E6<<" - "<<e3<<" = "<<sE9E3<<std::endl;
    assert(sE9E3.Get().Value()==6);
    isSame = std::is_same_v<decltype(sE9E3.Get())::UnitType,Sum>;
    assert(isSame);
	
	//~ auto sE9E3 = Subtraction::Calculate(pE3E6,e3);
    //~ std::cout<<pE3E6<<" - "<<e3<<" = "<<sE9E3<<std::endl;
    //~ assert(sE9E3.Get().Get().Value()==6);
    //~ isSame = std::is_same_v<decltype(sE9E3.Get().Get())::UnitType,Sum>;
    //~ assert(isSame);
	
	
	std::cout<<"END"<<std::endl;
    return 0;
}
