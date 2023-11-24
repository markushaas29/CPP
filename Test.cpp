#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include "Logger/Logger.hpp"
#include "Quantity/ToQuantity.hpp"
#include "Common/DateTimesTest.cpp"
#include "Quantity/Test.cpp"
#include "Matrix/Test.cpp"
#include "CSV/ElementTest/ElementTest.cpp"


int main()
{   
	std::cout<<"START"<<std::endl;

	DateTest dt;
	dt.Run();
	
	QTest qt;
	qt.Run();
	
	MatrixTest mt;
	mt.Run();

	ElementTest et;
	et.Run();
	
	std::cout<<"END"<<std::endl;
	return 0;
}
