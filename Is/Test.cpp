#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Is.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
		
	Is<CompileTime,3==3>()(2 == 3);
	//Is<CompileTime,2==3>()(2 == 3);
	
	auto s = "2==3";
	auto il = Is<Logging>(s);
	il(2 == 3);
	il(3 == 3);
	auto it = Is<Throwing>(s);
	it(2 == 3);
	//Is<int>()(2 == 3);
		
	std::cout<<"END"<<std::endl;

    return 0;
}
