#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "../Literal.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	constexpr Literal L{"Main"};
	constexpr const char C[] ="Main2";
	constexpr Literal L2{C};
	
	std::cout<<L<<std::endl;
	std::cout<<L2<<std::endl;
	std::cout<<L.Ptr()<<std::endl;
	std::cout<<L2.Ptr()<<std::endl;

	assert("Main"==L);
	assert(L=="Main");

	std::string m2("Main2");
	assert(m2==L2);
	assert(L2==m2);
	//assert(L2==L2);

	std::cout<<"END"<<std::endl;

    return 0;
}
