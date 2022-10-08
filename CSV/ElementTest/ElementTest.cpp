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
	
    return 0;
}
