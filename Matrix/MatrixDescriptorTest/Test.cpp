#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixCreator.hpp"
#include "../MatrixDescriptor.hpp"
#include "../Matrix_Impl.hpp"
#include "../../Common/ShortNames.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using MS3 = MatrixDescriptor<3,double>;
	using MS2 = MatrixDescriptor<2,std::string,std::tuple<std::unique_ptr<int>,std::string>>;
	using MS1 = MatrixDescriptor<1,std::string,DT>;
	
	MS2 ms2{{2,2}};
	MS2 ms254{{5,4}};
	assert(ms254.Rows()==5);
	assert(ms254.Cols()==4);
	assert(ms254.Size()==20);

	try	{	assert(9==9);	}
	catch(...){	}

	std::cout<<"END"<<std::endl;
//
    return 0;
}
