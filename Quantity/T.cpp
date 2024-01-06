#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Test.cpp"
#include "QuantityRatioTest.cpp"

int main()
{
	QTest sut;
	QRTest sut2;

	sut.Run();
	sut2.Run();

	return 0;
}

