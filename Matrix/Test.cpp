#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixDescriptor.hpp"
#include "../Common/ShortNames.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using MS2 = MatrixDescriptor<2,std::string>;
	using M1 = Matrix<1>;
	using M0 = Matrix<0,MS2>;
	MS2 ms2;
	std::cout<<M3::LiteralType<<std::endl;
	std::cout<<M1::LiteralType<<std::endl;

	M1 m1{
		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)}	
	};
	assert(m1.Rows()==5);
	assert(m1.Cols()==1);
//
//	M2 m2
//	{
//		{std::make_shared<int>(1)},
//		{std::make_shared<int>(2)},
//	};
//	assert(m2.Rows()==2);
//	assert(m2.Cols()==1);
//	assert(m2.Size()==2);
//
//	auto m20 = m2[0];
//	assert(m20.Size()==1);
//
//	auto r11 = m2.Row(0);
//	assert(*r11[0]==1);
//
//	M2 m35 {
//		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)},
//		{std::make_shared<int>(6),std::make_shared<int>(7),std::make_shared<int>(8),std::make_shared<int>(9),std::make_shared<int>(10)},
//		{std::make_shared<int>(11),std::make_shared<int>(12),std::make_shared<int>(13),std::make_shared<int>(14),std::make_shared<int>(15)},
//	};
//	std::cout<<"Rows: "<<m35.Rows()<<std::endl;
//	std::cout<<"Columns: "<<m35.Cols()<<std::endl;
//	std::cout<<"Columns: "<<*m35(1,0)<<std::endl;
//	assert(m35.Size()==15);
//	auto r1 = m35.Row(1);
//
//	auto c1 = m35.Col(1);
//	assert(*r1[0]==6);
//	assert(*c1[0]==2);
//	assert(*m35(1,0)==6);
//	assert(*m35(0,0)==1);
//	assert(*m35(2,0)==11);
//	assert(*m35(2,4)==15);
//	
//
//	M3 m3 {
//		{
//			{std::make_shared<int>(1),std::make_shared<int>(2)},
//			{std::make_shared<int>(3),std::make_shared<int>(4)}
//		},
//		{
//			{std::make_shared<int>(6),std::make_shared<int>(7)},
//			{std::make_shared<int>(8),std::make_shared<int>(9)}},
//	};
//	auto d = m3[0];//.Descriptor();
//	
//	auto dd = d[1];//.Descriptor();
//	auto ddd = dd[1];//.Descriptor();
//	std::cout<<"Matrix Element "<<d.Rows()<<"\n";
//	//assert(dd.Size()==1);
//
//	//assert(*(*ddd)==4);
//	assert(d.Rows()==2);
//	assert(d.Cols()==2);
//	assert(d.Size()==4);
//	auto db = m3[0].Descriptor();
//	assert(db.Stride(1)==1);
//	assert(db.Extent(0)==2);
//	assert(db.Extent(1)==2);
//	assert(db.Extent(2)==2);
//	assert(db.Stride(0)==2);
//	auto i = db.Strides();
//	assert(*m3(0,1,1)==4);
//	assert(*m3(1,0,1)==7);
//	assert(*m3(1,1,1)==9);
//
//	auto v1 = std::make_shared<ValueCell<int>>(1);
//	auto q1 = std::make_shared<QuantityCell<Q>>(1);
//	std::vector<std::shared_ptr<MatrixCell>> mcv = {v1, q1};
//
//	for(auto c : mcv)
//		c->Display(std::cout);
//	auto vc1 = v1;
//
//	std::cout<<v1<<vc1<<std::endl;
//	std::cout<<*v1<<*vc1<<std::endl;
//	vc1->value=11;
//	std::cout<<*v1<<*vc1<<std::endl;
//
//	auto uv1 = vc1->Clone();
//	auto duv1 = dynamic_cast<ValueCell<int>*>(uv1.get());
//	duv1->value=12;
//	std::cout<<*duv1<<*v1<<std::endl;
//
//	ms2.AddRow();
//	ms2.AddRow();
//	std::cout<<"E"<<ms2.Extent(0)<<std::endl;
//	assert(ms2.Extent(0)==2);
//	ms2.AddRow();
//	assert(ms2.Extent(0)==3);
//	M2 m2ms2(ms2,r1);
//	M2 m2I(10,20);
//	//m3(3,1,1);
//	auto e0 =m2I.Extent(0);
//	//std::cout<<"E"<<m2ms2[0]<<std::endl;
//	assert(e0==10);
//	std::cout<<"END"<<std::endl;
//
    return 0;
}
