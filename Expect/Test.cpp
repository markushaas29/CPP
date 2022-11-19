#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Expect.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
		
	expect<ErrorAction::Logging>(2 == 3);
	expect<ErrorAction::Throwing>(2 == 3);
		
	std::cout<<"END"<<std::endl;

    return 0;
}
