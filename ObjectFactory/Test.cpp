#include <iostream>
#include "Factory.hpp"
#include "../Matrix/MatrixCategory.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	Factory<int> f;
	Factory<int,std::function<int*()>> fi;

	std::string s;
	f(s,"");

	Factory<IMatrixCategory<std::string>> fm;
	std::cout<<"END"<<std::endl;
   
	return 0;
}
