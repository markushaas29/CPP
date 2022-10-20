#include <iostream>
#include "Logger.hpp"
#include "Logger.hpp"
#include <typeinfo>
#include <typeindex>
#include "../Wrapper/Wrapper.hpp"


int main()
{       
    std::cout<<"START"<<std::endl;
	
	Logger::Log<Info>("Test Info");
	Logger::Log<Warning>("Test Warning");
	Logger::Log<Error>("Test Error");
	Logger::Log<Success>("Test Error");
	
	LogExpression le1{1};
	LogExpression le2{2};
	LogExpression le3{3};
	
	Logger::Log(1,2,3.9,"Test");
	Logger::Log<Error>(1,2,3.9,"Test");
	Logger::Log<';', Debug >(1,2,3.9,"Test");
	
	std::cout<<"END"<<std::endl;

    return 0;
}
