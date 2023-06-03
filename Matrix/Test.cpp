#include <iostream>
#include <cassert> 
#include <memory> 
#include "Matrix.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	std::cout<<M3::LiteralType<<std::endl;

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
	std::cout<<"Columns: "<<m35.Columns()<<std::endl;
	std::cout<<"Columns: "<<*m35(1,0)<<std::endl;
	assert(*m35(1,0)==6);
	assert(*m35(0,0)==1);
	assert(*m35(2,0)==11);
	assert(*m35(2,4)==15);
//	M3 m3 {
//		{{1,2},{3,4}},
//		{{5,6},{7,8}},
//		{{9,10},{11,12}}
//	};
//
//	auto r = m3.Row(1);
//
	std::cout<<"M\n "<<m35<<std::endl;
	std::cout<<"END"<<std::endl;

    return 0;
}
