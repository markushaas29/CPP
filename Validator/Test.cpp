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
	SizeValidator<22>::Check(c);
	
	expect<Info>(IsLetter('A'), "A Should be true");	
	expect<Info>(IsLetter('a'), "a Should be true");	
	expect<Info>(IsLetter('?'), "ok ?");	
	expect<Info>(IsLetter('z'), "z Should be true");	
	expect<Info>(IsLetter('Z'), "Z Should be true");	
	expect<Info>(IsNum('0'), "Z Should be true");	
	expect<Info>(IsNum('9'), "Z Should be true");	
	std::cout<<"END"<<std::endl;

    return 0;
}
