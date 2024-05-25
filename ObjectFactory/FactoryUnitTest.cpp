#include <iostream>
#include "Factory.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/IMatrixQuery.hpp"
#include "../Matrix/MatrixQuery.hpp"
#include "../Matrix/MatrixCategory.hpp"
#include "../Matrix/MatrixMultiCategory.hpp"

class FactoryUnitTest
{
	public:
int Run()
{
	std::cout<<"START FactoryUnit"<<std::endl;
	FactoryUnit fu;
	std::istringstream is("1 2 3.4 km");
	is>>fu;
	assert(fu.Id()=="1");
	assert(fu.Arg()=="2");
	std::cout<<"factory "<<fu<<std::endl;
	std::cout<<"END factory"<<std::endl;
   
	return 0;
}
};
