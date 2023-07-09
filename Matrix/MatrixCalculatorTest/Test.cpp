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
	using MS3 = MatrixDescriptor<3,double>;
	using MS2 = MatrixDescriptor<2,std::string,std::tuple<std::unique_ptr<int>,std::string>>;
	using MS1 = MatrixDescriptor<1,std::string,std::string>;
	using M3D = Matrix<3,MS3>;
	using M1S = Matrix<1,MS1>;
	using M2M = Matrix<2,MS2>;
	using MC2 = MatrixCreator<M2M>;
	using M1 = Matrix<1>;
	MS2 ms2{{2,2}};
	MS2 ms254{{5,4}};
	assert(ms254.Rows()==5);
	assert(ms254.Cols()==4);
	assert(ms254.Size()==20);
	std::cout<<M3::LiteralType<<std::endl;
	std::cout<<M1::LiteralType<<std::endl;

	M1 m1{
		{1,2,3,4,5}	
	};

	M2 m34 {
		{1, 2, 3, 4},
		{6, 7, 8, 9},
		{11, 12, 13, 14},
		{11, 12, 13, 14}
	};

	M2 m35 {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9,10},
		{11, 12, 13, 14, 15},
	};
	std::cout<<"Matrix m3 "<<(m35 )<<"\n";
	std::cout<<"Matrix m3 "<<(m35 + 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 - 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 * 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 / 2)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 + m35)<<"\n";
	std::cout<<"Matrix m3 "<<(m35 - m35)<<"\n";
	auto m35m1 = m35 * m1;
	std::cout<<"Matrix m35m1 "<<m35m1<<"\n";
	std::cout<<"Rows: "<<m35.Rows()<<std::endl;
	std::cout<<"Columns: "<<m35.Cols()<<std::endl;
	std::cout<<"Columns: "<<*m35(1,0)<<std::endl;
	assert(m35.Size()==15);
	auto r1 = m35.Row(1);
	auto c1 = m35.Col(1);
	assert(*r1[0]==6);
	assert(*c1[0]==2);
	assert(*m35(1,0)==6);
	assert(*m35(0,0)==1);
	assert(*m35(2,0)==11);
	assert(*m35(2,4)==15);
	bool t = false;
	try	{ m35.AddRow({2});	}
	catch(...){ t = true;	}
	assert(t);
	auto m2Move(std::move(m35));
	std::cout<<"Matrix m2Move "<<m2Move<<"\n";
	auto m2AMove = std::move(m2Move);
	std::cout<<"Matrix m2Move "<<m2AMove<<"\n";

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
