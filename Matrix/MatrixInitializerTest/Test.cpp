#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixCreator.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixInitializer.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using MS3 = MatrixDescriptor<3,double>;
	using MS2 = MatrixDescriptor<2,std::string>;
	using MS1 = MatrixDescriptor<1,std::string>;
	using MQ1DS = MatrixDescriptor<1,double>;
	using M3D = Matrix<3,MS3>;
	using M1S = Matrix<1,MS1>;
	using M1Q = Matrix<1,MQ1DS>;
	using M2M = Matrix<2,MS2>;
	using MC2 = MatrixCreator<M2M>;
	using M1 = Matrix<1>;
	
	MC2 mc2{};
	mc2.Create("");
	auto mc2m = mc2.Get();
	auto m15 = mc2m[15];
  	std::cout<<"\nVal: "<<mc2m[15][5]()<<std::endl;
	assert(mc2m.Rows()==125);
	assert(mc2m.Cols()==19);

	auto mI1 = MatrixInitializer<1>();

	std::vector<int> v1{1,2,3,4};
	std::vector<std::vector<int>> v2{{1,2},{3,4}};
	std::vector<std::vector<std::vector<int>>> v3{{{1},{2}},{{3},{4}}};

	auto mi1 = Init(v1);
	auto mi2 = Init(v2);
	auto mi3 = Init(v3);

	assert(decltype(mi1)::Order==1);
	assert(decltype(mi2)::Order==2);
	assert(decltype(mi3)::Order==3);

	std::cout<<"END"<<decltype(mi3)::Order<<std::endl;
   
	return 0;
}
