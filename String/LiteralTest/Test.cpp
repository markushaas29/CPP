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

	const auto P = L.Ptr();

	std::cout<<"END"<<std::endl;

    return 0;
}
