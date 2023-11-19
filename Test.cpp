#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include "Logger/Logger.hpp"
#include "Quantity/ToQuantity.hpp"
#include "Common/DateTimesTest.cpp"
#include "Quantity/Test.cpp"


int main()
{   
	std::cout<<"START"<<std::endl;

	DateTest dt;
	dt.Run();
	
	QTest qt;
	qt.Run();

	std::cout<<"END"<<std::endl;
	return 0;
}
