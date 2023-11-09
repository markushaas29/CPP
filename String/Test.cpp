#include <iostream>
#include "String_.hpp"
#include <typeinfo>
#include <typeindex>
using namespace String_;

int main()
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

	std::cout<<"Size2: "<<d<<std::endl;
    
    return 0;
}
