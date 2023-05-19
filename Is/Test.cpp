#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Is.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
		
	Is<CompileTime>()(2 == 3);
	Is<Logging>()(2 == 3);
	Is<Logging,Throwing>()(3 == 3);
	Is<Logging,Throwing>()(2 == 3);
	//Is<int>()(2 == 3);
		
	std::cout<<"END"<<std::endl;

    return 0;
}
