#include <iostream>
#include "Factory.hpp"
#include "../Matrix/MatrixCategory.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	Factory<int> f;
	Factory<int,std::function<int*()>> fi;

	std::string s;
	//try {f(s,s); } catch(...) { }

	Factory<IMatrixCategory<std::string>> fm;
	fm.Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==1);
	fm.Register("C",[](std::string_view s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
	assert(fm.Size()==2);

	auto eqp = fm["EQ"];
	auto eq2 = eqp("2");
	assert((*eq2)("2"));
	std::cout<<"END"<<std::endl;
   
	return 0;
}
