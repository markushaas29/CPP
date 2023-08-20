#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixInitializer.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;

class MatrixInitializerTest
{
	public:
		int Run()
		{
			std::cout<<"START"<<std::endl;
		
			auto u22 = std::string{ "/home/markus/Dokumente/cpp/Matrix/MatrixProjectorTest/UErr_2022.csv" };
			auto m22r = MatrixReader(u22);

			std::cout<<"MS 22"<<m22r.M<2>()[1][4]<<std::endl;
			auto m22 = m22r.M<2>();
			auto mS22 = m22.Slices(10);
			std::cout<<"MS 22"<<mS22<<std::endl;

			auto ms22F = MatrixFilter<decltype(mS22)>(mS22);
			//auto fmS22 = ms22F(0,[&](const auto& s) { return s == "04.01.2022";});
			//std::cout<<ms22F(0,[&](const auto& s) { return s == "04.01.2022";});
		
			std::cout<<"END"<<std::endl;

			return 0;
		}
};

int main()
{
	MatrixInitializerTest init;

	init.Run();

	return 0;
}
