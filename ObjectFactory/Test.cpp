#include <iostream>
#include "Factory.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	Factory<int> f;
	Factory<int,std::function<int*()>> fi;

	f("");

	std::cout<<"END"<<std::endl;
   
	return 0;
}
