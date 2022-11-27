#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Validator.hpp"
#include "../Expect/Expect.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
		
	auto r1 = RangeValidator{1,5};
	expect<Info>(r1(6) != 6, "Should be 1");	
	
	std::cout<<"END"<<std::endl;

    return 0;
}
