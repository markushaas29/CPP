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
	fm.Register("EQ",[](std::string_view s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==1);
	std::cout<<"END"<<std::endl;
   
	return 0;
}
