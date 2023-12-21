#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Singleton.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

class SingletonTest
{
	public:
		int Run()
		{
			std::cout<<"START Singleton"<<std::endl;

			Singleton<int>::Get();

			std::cout<<"END Singleton"<<std::endl;

			return 0;
		}
};

