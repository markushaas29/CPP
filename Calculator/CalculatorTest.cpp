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
	
	std::cout<<Multiplication::Calculate(3,3)<<std::endl;
	std::cout<<Addition::Calculate(e6,e3)<<std::endl;
	std::cout<<Subtraction::Calculate(e6,e3)<<std::endl;
	std::cout<<Multiplication::Calculate(e6,e3)<<std::endl;
	std::cout<<Division::Calculate(e6,e3)<<std::endl;
	std::cout<<QuantityRatio::Calculate(e6,e3,e9)<<std::endl;
	std::cout<<QuantityRatio::Calculate(e9,e3,e9)<<std::endl;
	
	
	std::cout<<"END"<<std::endl;
    return 0;
}
