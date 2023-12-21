#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Repository.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

class RepositoryTest
{
	public:
		int Run()
		{
			std::cout<<"START Repository"<<std::endl;

//			Repository::Get().Is();

			std::cout<<"END Repository"<<std::endl;

			return 0;
		}
};

