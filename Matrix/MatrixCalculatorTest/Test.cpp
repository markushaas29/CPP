#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixCreator.hpp"
#include "../MatrixDescriptor.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using M1 = Matrix<1>;
	using M3DS = MatrixDescriptor<3,double>;
	using M2DS = MatrixDescriptor<2,double>;
	using M1DS = MatrixDescriptor<1,double>;
	using M3D = Matrix<3,M3DS>;
	using M2D = Matrix<2,M2DS>;
	using M1D = Matrix<1,M1DS>;

	M1 m1{
		{1,2,3,4,5}	
	};

	M2 m44 {
		{1, 2, 3, 4},
		{6, 7, 8, 9},
		{1, 2, 3, 4},
		{6, 7, 8, 9}
	};
	
	M1D m1d{
		{1.1,2.2,3.3,4.4}	
	};

	std::cout<<"Matrix m3 "<<(m1d * m1d )<<"\n";
	
	M2D m2d {
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4}
	};
	std::cout<<"Matrix m3 "<<(m2d * m2d )<<"\n";
	std::cout<<"Matrix m3 "<<(m2d * m2d * m1d)<<"\n";
	//std::cout<<"Matrix m3 "<<(m1d * m2d)<<"\n";

	M2 m35 {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9,10},
		{11, 12, 13, 14, 15},
	};
	std::cout<<"Matrix m3 "<<(m35 )<<"\n";
//	std::cout<<"Matrix m3 "<<(m35 + 2)<<"\n";
//	std::cout<<"Matrix m3 "<<(m35 - 2)<<"\n";
//	std::cout<<"Matrix m3 "<<(m35 * 2)<<"\n";
//	std::cout<<"Matrix m3 "<<(m35 / 2)<<"\n";
//	std::cout<<"Matrix m3 "<<((m35 + m35)+(m35 + m35))<<"\n";
	//std::cout<<"Matrix m3 "<<((m35 + m35)+(m35 + m35) + m35)<<"\n";
	std::cout<<"Matrix m3 "<<(m35+m35 + m35 )<<"\n";
	auto mad = m35 + m35 -m35 +m35; 
	std::cout<<"Matrix m3 Sub"<<(m35 - m35 - m35 + m35 -m35 +m35 )<<"\n";
	auto m35m1 = m35 * m1;
	std::cout<<"Matrix m35m1 "<<m35m1<<"\n";

	M3 m3 {
		{
			{1,2},
			{3,4}
		},
		{
			{6,7},
			{8,9}
		},
	};

	std::cout<<"END"<<std::endl;
   
	return 0;
}
