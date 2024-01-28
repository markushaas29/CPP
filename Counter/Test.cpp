#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "CounterHeaders.hpp"

class CounterTest
{
	public:
		int Run()
		{
			std::cout<<"START COunterTest"<<std::endl;
			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
