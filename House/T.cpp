#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include <chrono> 
#include "Test.cpp"

int main()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	MatrixVisitorTest analyzer;

	analyzer.Run();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
	return 0;
}

