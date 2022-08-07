#include <iostream>
#include <vector>
#include "Calculator.hpp"
#include "Operations.hpp"
#include "../Home/Counter.hpp"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Home/Stage.hpp"
#include "../Quantity/Quantity.h"

int main()
{       
    std::cout<<"START"<<std::endl;

	auto e3 = Quantity<Sum>{3};
	auto e6 = Quantity<Sum>{6};
	auto e9 = Quantity<Sum>{9};
	auto s2 = Quantity<Scalar>{2};
	
	auto m1 = Addition::Calculate(e6,e9)+Addition::Calculate(e6,e9)+Addition::Calculate(e6,e9);
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
	
	//~ std::cout<<Multiplication::Calculate(Multiplication::Calculate(Multiplication::Calculate(m1,e6),e3),s2)<<std::endl;
	std::cout<<"\n---------------------Addition---------------------"<<std::endl;
	std::cout<<(e6+m1+e6)<<std::endl;
	std::cout<<(Addition::Calculate(m1,m1)+Addition::Calculate(m1,m2)+m1)<<std::endl;
	std::cout<<"\n---------------------Subtra---------------------"<<std::endl;
	std::cout<<(Subtraction::Calculate(m1,m1)+Subtraction::Calculate(m1,m2)+m1)<<std::endl;
	std::cout<<(m1-(e6-m1)-e6)<<std::endl;
	std::cout<<"\n---------------------Multplication---------------------"<<std::endl;
	std::cout<<(e6/Multiplication::Calculate(m1,m2)*Multiplication::Calculate(m1,e6)*m2)<<std::endl;
	std::cout<<"\n---------------------Division---------------------"<<std::endl;
	std::cout<<(e6/Division::Calculate(d,d)/Multiplication::Calculate(m1,e6)/m2)<<std::endl;
	
	
	std::cout<<"END"<<std::endl;
    return 0;
}
