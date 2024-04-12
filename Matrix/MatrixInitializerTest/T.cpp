#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Test.cpp"

int main()
{
	std::cout<<"START"<<std::endl;

    MatrixInitializerTest calc;     
    calc.Run();     
	std::cout<<"END"<<std::endl;
   
	return 0;
}
