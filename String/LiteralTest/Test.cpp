#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "../Literal.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	constexpr Literal L{"Main"};
	//constexpr Literal L2{"2ain"};
	std::cout<<L<<std::endl;
	std::cout<<L.Ptr()<<std::endl;

	const auto P = L.Ptr();

	std::cout<<"END"<<std::endl;

    return 0;
}
