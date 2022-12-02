#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Validator.hpp"
#include "../Expect/Expect.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
		
	expect<Info>(RangeValidator<uint,1,5>::Check(6) != 6, "Should be 1");	
	
	std::string s = "TEST";
	SizeValidator<const char*,5>::Check(s.c_str());
	
	std::cout<<"END"<<std::endl;

    return 0;
}
