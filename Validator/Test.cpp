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
	const char* c1 = "DE975005000000A3200029";
	std::cout<<length(s.c_str())<<std::endl;
	std::cout<<length(c)<<std::endl;
	static constexpr IBAN iban{"#E97500500000003200029"};
	static constexpr IBAN iban2{"1X00A00000A00000000000"};	
	SizeValidator<22>::Check(c);
	
	expect<Info>(IsLetter('A'), "A Should be true");	
	expect<Info>(IsLetter('a'), "a Should be true");	
	expect<Info>(IsLetter('?'), "ok ?");	
	expect<Info>(IsLetter('z'), "z Should be true");	
	expect<Info>(IsLetter('Z'), "Z Should be true");	
	expect<Info>(IsNum('0'), "Z Should be true");	
	expect<Info>(IsNum('9'), "Z Should be true");	
	
	const char* c2 = "DE97500500A00003200029";
	//~ expect<Info>(isIban(c), c);	
	//~ expect<Info>(isIban(c2), c2);	
	
	expect<Info>(NumberValidator::Check('9'), "9 Should be true");	
	expect<Info>(NumberValidator::Check('a'), "a Should not be true");
		
	expect<Info>(LowerCaseLetterValidator::Check('a'), "a Should be true");	
	expect<Info>(LowerCaseLetterValidator::Check('z'), "z Should be true");	
	expect<Info>(LowerCaseLetterValidator::Check('A'), "A Should not be true");	
	expect<Info>(LowerCaseLetterValidator::Check('Z'), "Z Should not be true");	
	expect<Info>(LowerCaseLetterValidator::Check('9'), "9 Should not be true");	
	
	expect<Info>(CapitalLetterValidator::Check('a'), "a Should not be true");	
	expect<Info>(CapitalLetterValidator::Check('z'), "z Should not be true");	
	expect<Info>(CapitalLetterValidator::Check('A'), "A Should be true");	
	expect<Info>(CapitalLetterValidator::Check('Z'), "Z Should be true");	
	expect<Info>(CapitalLetterValidator::Check('9'), "9 Should not be true");	
	expect<Info>(LetterValidator::Check('a'), "a Should be true");	
	expect<Info>(LetterValidator::Check('A'), "A Should be true");	
	expect<Info>(LetterValidator::Check('#'), "9 Should not be true");	
	
	if(NumberValidator::Check('9'))
		std::cout<<"9 is number"<<std::endl;
	else
		std::cout<<"BOOL2"<<std::endl;
	
	
	
	std::cout<<"iban"<<iban.Value()<<std::endl;
	std::cout<<"iban"<<iban2.Value()<<std::endl;
	std::cout<<"END"<<(3==1)<<std::endl;

    return 0;
}
