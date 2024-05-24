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
	std::istringstream is("123.4 km");
	is>>fu;
	std::cout<<"END factory"<<std::endl;
   
	return 0;
}
};
