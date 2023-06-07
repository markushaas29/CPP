#include <iostream>
#include <cassert> 
#include <memory> 
#include "Matrix.hpp"
#include "Matrix_Slice.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using MS2 = MatrixSlice<2>;
	using M1 = Matrix<1>;
	using M0 = Matrix<0>;
	std::cout<<M3::LiteralType<<std::endl;
	std::cout<<M1::LiteralType<<std::endl;
	std::cout<<M0::LiteralType<<std::endl;

	M0 m0{std::make_shared<int>(1)};
	assert(m0.Rows()==0);
	assert(m0.Rows()==0);
	assert((*m0())==1);

	M1 m1{		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)}	};
	std::cout<<"Rows: "<<m1.Rows()<<std::endl;
	std::cout<<"Columns: "<<m1.Cols()<<std::endl;
	assert(m1.Rows()==5);
	assert(m1.Cols()==1);

	M2
	{
		{std::make_shared<int>(1)},
		{std::make_shared<int>(2)},
	};

//	M2 m2 {
//		{1,2},
//		{3,4},
//	};

//	M2 m21 {
//		{1,2,1,2},
//		{3,4,3,4},
//		{3,4,3,4},
//	};
	M2 m35 {
		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)},
		{std::make_shared<int>(6),std::make_shared<int>(7),std::make_shared<int>(8),std::make_shared<int>(9),std::make_shared<int>(10)},
		{std::make_shared<int>(11),std::make_shared<int>(12),std::make_shared<int>(13),std::make_shared<int>(14),std::make_shared<int>(15)},
	};
	std::cout<<"Rows: "<<m35.Rows()<<std::endl;
	std::cout<<"Columns: "<<m35.Cols()<<std::endl;
	std::cout<<"Columns: "<<*m35(1,0)<<std::endl;
	auto r1 = m35.Row(1);

	for(auto x : r1)
		std::cout<<*x<<std::endl;

	assert(*r1[0]==6);
	assert(*m35(1,0)==6);
	assert(*m35(0,0)==1);
	assert(*m35(2,0)==11);
	assert(*m35(2,4)==15);
	M3 m3 {
		{
			{std::make_shared<int>(1),std::make_shared<int>(2)},
			{std::make_shared<int>(3),std::make_shared<int>(4)}
		},
		{
			{std::make_shared<int>(6),std::make_shared<int>(7)},
			{std::make_shared<int>(8),std::make_shared<int>(9)}},
	};
//
//	auto r = m3.Row(1);
//
	std::cout<<"M35\n "<<m35<<std::endl;
	std::cout<<"M0\n "<<m0<<std::endl;
	assert(*m3(0,1,1)==4);
	assert(*m3(1,0,1)==7);
	assert(*m3(1,1,1)==9);


	MS2 ms2;
	M2 m2ms2(ms2);
	M2 m2I(10,20);
	//m3(3,1,1);
	auto e0 =m2I.Extent(0);
	std::cout<<"E"<<e0<<std::endl;
	assert(e0==10);
	std::cout<<"END"<<std::endl;

    return 0;
}
