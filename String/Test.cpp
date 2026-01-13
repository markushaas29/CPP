#include <iostream>
#include "String_.hpp"
#include "Format.hpp"
#include <typeinfo>
#include <typeindex>
using namespace String_;

class String_Test
{
	public:
int Run()
{       

	double d = 5.130000;

	auto s = TrimDouble(d);
	assert(s == "5.13");

	s = " 123";
	assert(Trim(s) == "123");
	s = "    123 ";
	assert(Trim(s) == "123");
	s = "123        ";
	assert(Trim(s) == "123");
	s = "     123 456 ";
	assert(Trim(s) == "123 456");
	s = "A\r\nBC\r\n";
	assert(Contains(s,"\r\n"));
	std::string s2 = RemoveAll(s,"\r");
	assert(s2=="A\nBC\n");
	assert(!Contains(s2,"\r"));

	std::cout<<"Size2: "<<d<<NL(4)<<std::endl;
    
    return 0;
}
};
