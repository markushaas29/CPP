#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Validator.hpp"
#include "../Expect/Expect.hpp"
#include "../CSV/Elements.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
		
	expect<Info>(RangeValidator<uint,1,5>::Check(6) != 6, "Should be 1");	
	
	std::string s = "TEST";
	const char* c = "DE97500500000003200029";
	std::cout<<length(s.c_str())<<std::endl;
	std::cout<<length(c)<<std::endl;
	static constexpr IBAN iban{"DE97500500000003200029"};
	static constexpr IBAN iban2{"XX00000000000000000000"};	
	SizeValidator<const char*,22>::Check(c);
	
	std::cout<<"END"<<std::endl;

    return 0;
}
