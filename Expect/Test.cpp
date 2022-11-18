#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Expect.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
		
	expect<ErrorAction::Logging>(2 == 3,4);
		
	std::cout<<"END"<<std::endl;

    return 0;
}
