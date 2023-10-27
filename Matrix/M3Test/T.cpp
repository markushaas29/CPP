#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Test.cpp"
#include "ProjectionCalc.cpp"
using namespace ShortNames;

int main()
{
	MatrixProjectorTest analyzer;
	MatrixProjectorCalculationTest calc;

	analyzer.Run();
	calc.Run();

	return 0;
}

