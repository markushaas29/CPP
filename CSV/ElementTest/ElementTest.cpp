#include <iostream>
#include <cassert>
#include "../CSV.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	auto s = std::string("123");
    std::cout<<s<<std::endl;

	auto e = Element(s.c_str());
	assert(s.size()==e.Value().size());
	assert(s==e.Value());
    std::cout<<e.Value().size()<<std::endl;
    std::cout<<e.Value()<<std::endl;
	
	auto s1 = std::string("DE123DE");
	auto i = IBAN(s1.c_str());
	assert(s1.size()==i.Value().size());
	assert(s1==i.Value());
    std::cout<<i.Value()<<std::endl;
	
    return 0;
}
