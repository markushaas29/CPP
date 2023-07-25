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

	auto aA = m44.Apply([&](const auto& e1){ return *e1 + 10; });
	std::cout<<"Matrix mA "<<aA<<"\n";
	auto m44m1 = m44*m1;
	std::cout<<"Matrix m44*m1 "<<m44m1<<"\n";
	assert(*m44m1(0)==30);
	auto a5 = m44 + 5;
	assert((int)*a5(0,3)==9);

	auto s5 = m44 - 5;
	assert((int)*s5(0,3)==-1);
	
	auto m5 = m44 * 5;
	assert((int)*m5(0,3)==20);
	
	auto d2 = m44  / 2;
	assert((int)*d2(0,3)==2);
	
	M1D m1d{
		{1.1,2.2,3.3,4.4}	
	};



	std::cout<<"Matrix m1 * m1 \n"<<(m1d * m1d )<<"\n";
	std::cout<<"Matrix m2 + m2 \n"<<(m44 + m44 )<<"\n";
	std::cout<<"Matrix m2 + m2 \n"<<(a5 - m44 )<<"\n";
	std::cout<<"Matrix d2\n"<<d2<<"\n";
	auto m2SA = a5 - m44 + d2 ;
	std::cout<<"Matrix m25A \n"<<*m2SA(0,3)<<"\n";
	assert((int)*m2SA(0,3)==7);
	std::cout<<"Matrix m2 + m2 \n"<<m2SA<<"\n";
	
	M2D m2d {
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4}
	};
	//std::cout<<"Matrix m2 EL(0.3) "<<(d2.ElAt(0,3))<<"\n\n";
	//std::cout<<"Matrix m2 EL(0.3) "<<(d2.ElAt(0,3))<<"\n\n";
	std::cout<<"Matrix m3 "<<(m2d * m2d )<<"\n";
	std::cout<<"Matrix m3 "<<(m2d * m2d * m1d)<<"\n";
	//std::cout<<"Matrix m3 "<<(m1d * m2d)<<"\n";

	M2 m35 {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9,10},
		{11, 12, 13, 14, 15},
	};
	std::cout<<"Matrix m3 "<<(m35 )<<"\n";
	std::cout<<"Matrix m3 "<<(m35+m35 + m35 )<<"\n";
	auto mad = m35 + m35 -m35 +m35; 
  	auto mop = ((m35 * 2)- m35 - (m35 + 5) + (m35 - 6) -m35 +m35 );
  	std::cout<<"Matrix m3 Sub"<<mop<<"\n";
  	assert((int)*mop(0,0)==-10);
  	std::cout<<"Matrix m3 Sub 00"<<*mop(0,0)<<"\n";
  
	auto m35m1 = m35 * m1;
	std::cout<<"Matrix m35m1 "<<m35m1<<"\n";
  	std::cout<<"Matrix m3 + 2"<<(m35 + 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 - 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 * 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 / 2)<<"\n";

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
