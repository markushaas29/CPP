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
	using QS = Quantity<Scalar,Pure,double>;

	M1 m1{
		{1,2,3,4,5}	
	};
	M2 m35 {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9,10},
		{11, 12, 13, 14, 15},
	};
	
	std::cout<<"Matrix m3 "<<(m35 )<<"\n";
	std::cout<<"Matrix m3 "<<(m35+m35 + m35 )<<"\n";
	auto mad = m35 + m35 -m35 +m35; 
  	auto mop = ((m35 * 2)- m35 - (m35 + 5) + (m35 - 6) -m35 +m35 );
  	assert((int)mop[0][0]==-10);
  	int i00 = mop[0][0];
  	assert(i00==-10);
	auto i00q = mop[0][0].Get();
  	std::cout<<"Matrix m3 Sub 00"<<mop[0][0]()<<"_"<<i00q<<"\n";
	assert(i00q.Value()==-10.0);
	bool is = std::is_same_v<QS, decltype(i00q)>;
	assert(is);
	auto m35m1 = m35 * m1;
	std::cout<<"Matrix m35m1 "<<m35m1<<"\n";
	

	M2 m44 {
		{1, 2, 3, 4},
		{6, 7, 8, 9},
		{1, 2, 3, 4},
		{6, 7, 8, 9}
	};

	auto m44m44 = m44 * m44;
	std::cout<<"Matrix m44 + m44 "<<(int)m44m44[0][0]<<"\n";
	assert((int)m44m44[0][0]==40);
	auto aA = m44.Apply([&](const auto& e1){ return *e1 + 10; });
	std::cout<<"Matrix mA "<<aA<<"\n";
	auto m44m1 = m44*m1;
	
	auto m1m1 = m1*m1;
	std::cout<<"Matrix m44*m1 "<<m44m1<<"\n";
	
	assert((int)(m44m1[0])==30);
	auto a5 = m44 + 5;
	assert((int)a5[0][3]==9);

	auto s5 = m44 - 5;
	assert((int)s5[0][3]==-1);
	
	auto m5 = m44 * 5;
	assert((int)m5[0][3]==20);
	
	auto d2 = m44  / 2;
	assert((int)d2[0][3]==2);
	
	M1D m1d{
		{1.1,2.2,3.3,4.4}	
	};
	auto m1dq = m1d[0].Get();
	assert(m1dq.Value()==1.1);
	std::cout<<"Matrix m1d * m1d \n"<<(m1d * m1d )<<"\n";
	std::cout<<"Matrix m2 * m2 \n"<<(m44 + m44 )<<"\n";
	std::cout<<"Matrix m2 + m2 \n"<<(a5 - m44 )<<"\n";
	std::cout<<"Matrix d2\n"<<d2<<"\n";
	auto m2SA = a5 - m44 + d2 ;
	assert((int)m2SA[0][3]==7);
	std::cout<<"Matrix m2 + m2 \n"<<m2SA<<"\n";
	
	M2D m2d {
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4},
		{1.1, 2.2, 3.3, 4.4}
	};

  	std::cout<<"Matrix m3 + 2"<<(m35 + 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 - 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 * 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 / 2)<<"\n";
	std::cout<<"Matrix int m1*m1 "<<m1m1<<"\n";

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

	auto clon = mop[0][0].Clone();
	std::cout<<"Clon "<<*clon<<"\n";

	auto m1Am1 = m1 + m1; 
	std::cout<<"ColSum:\n "<<m1Am1<<"\n";
	auto m2C = m44.ColSum(); 
	assert((int)m2C[0]==14);
	assert((int)m2C[2]==22);
	auto m2dC = m2d.ColSum();
	assert((double)m2dC[0]==4.4);
	assert((double)m2dC[2]==13.2);
	auto m1C = m1.ColSum(); 
	assert((int)m1C[0]==1);
	assert((int)m1C[2]==3);
	auto m35C = m35.ColSum(); 
	assert((int)m35C[0]==18);
	assert((int)m35C[1]==21);
	
	auto m2R = m44.RowSum(); 
	std::cout<<"ColSum:\n "<<m2R<<"\n";
	assert((int)m2R[0]==10);
	assert((int)m2R[1]==30);
	
	auto m35R = m35.RowSum(); 
	std::cout<<"ColSum:\n "<<m35R<<"\n";
	assert((int)m35R[0]==15);
	assert((int)m35R[1]==40);
	auto m1R = m1.RowSum(); 
	assert((int)m1R==15);
	std::cout<<"ColSum:\n "<<(m44 * m2C)<<"\n";

	std::cout<<"END"<<std::endl;
//   
	return 0;
}
