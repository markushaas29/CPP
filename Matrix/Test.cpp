#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixCreator.hpp"
#include "MatrixDescriptor.hpp"
#include "../Common/ShortNames.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using MS3 = MatrixDescriptor<3,double>;
	using MS2 = MatrixDescriptor<2,std::string,std::tuple<std::unique_ptr<int>,std::string>>;
	using MS1 = MatrixDescriptor<1,std::string,DT>;
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
		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)}	
	};
	assert(m1.Rows()==5);
	assert(m1.Cols()==1);
	auto e1 = m1.ElementsAt(4);
	assert(e1[0].Get()==5);
	
	auto m1a = m1.Apply([](const auto i) { return *i + 5; });
	auto e1a = m1a.ElementsAt(4);
	assert(e1a[0].Get()==10);
	auto m1o = m1a + 5;
	e1a = m1o.ElementsAt(4);
	assert(e1a[0].Get()==15);
	auto m1p = m1a + m1o;
	assert(m1p.ElementsAt(4)[0].Get()==25);

	M1S m1s{
		{std::make_shared<std::string>("1"),std::make_shared<std::string>("2")}	
	};
	assert(m1s.Rows()==2);
	assert(m1s.Cols()==1);
	
	std::vector<std::shared_ptr<std::string>> v1s{{std::make_shared<std::string>("3")}};
	m1s.AddRow(v1s);
	assert(m1s.Rows()==3);
	assert(*m1s(2)=="3");
	
	M2 m2
	{
		{std::make_shared<int>(1)},
		{std::make_shared<int>(2)},
	};
	assert(m2.Rows()==2);
	assert(m2.Cols()==1);
	assert(m2.Size()==2);


	auto m20 = m2[0];
	assert(m20.Size()==1);

	auto r11 = m2.Row(0);
	assert(*r11[0]==1);

	M2 m35 {
		{std::make_shared<int>(1),std::make_shared<int>(2),std::make_shared<int>(3),std::make_shared<int>(4),std::make_shared<int>(5)},
		{std::make_shared<int>(6),std::make_shared<int>(7),std::make_shared<int>(8),std::make_shared<int>(9),std::make_shared<int>(10)},
		{std::make_shared<int>(11),std::make_shared<int>(12),std::make_shared<int>(13),std::make_shared<int>(14),std::make_shared<int>(15)},
	};
	std::cout<<"Matrix m3 "<<m35<<"\n";
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
	try	{ m35.AddRow({std::make_shared<int>(2)});	}
	catch(...){ t = true;	}
	assert(t);
	
	M3 m3 {
		{
			{std::make_shared<int>(1),std::make_shared<int>(2)},
			{std::make_shared<int>(3),std::make_shared<int>(4)}
		},
		{
			{std::make_shared<int>(6),std::make_shared<int>(7)},
			{std::make_shared<int>(8),std::make_shared<int>(9)}},
	};
	auto d = m3[0];//.Descriptor();
	
	auto dd = d[1];//.Descriptor();
	auto ddd = dd[1];//.Descriptor();
	std::cout<<"Matrix Element Rows"<<d.Rows()<<"\n";
	//assert(dd.Size()==1);

	//assert(*(*ddd)==4);
	assert(d.Rows()==2);
	assert(d.Cols()==2);
	assert(d.Size()==4);
	auto db = m3[0].Descriptor();
	assert(db.Stride(1)==1);
	assert(db.Extent(0)==2);
	assert(db.Extent(1)==2);
	assert(db.Extent(2)==2);
	assert(db.Stride(0)==2);
	auto i = db.Strides();
	assert(*m3(0,1,1)==4);
	assert(*m3(1,0,1)==7);
	assert(*m3(1,1,1)==9);
	try	{ assert(*m3(2,1,1)==9); }	catch(...)	{	}

	try	{ assert(*m3(1,3,1)==9);	}	catch(...)	{	}
	
	try	{	assert(*m3(1,1,5)==9);	}catch(...)	{	}

	MC2 mc2{};
	mc2.Create("");
	auto mc2m = mc2.Get();
	
	std::cout<<"\nVal: "<<*mc2m(15,5)<<std::endl;
	//auto e15 = mc2m.ElementsAt(15);
	assert(mc2m.Rows()==125);
	assert(mc2m.Cols()==19);
	//std::vector<std::shared_ptr<MatrixCell>> mcv = {v1, q1};

	//auto vc1 = v1;

	//std::cout<<"V1"<<v1<<vc1<<std::endl;
	//std::cout<<*v1<<*vc1<<std::endl;
	//vc1->value=11;
	//std::cout<<*v1<<*vc1<<std::endl;

	//auto uv1 = vc1->Clone();
	//auto duv1 = dynamic_cast<ValueCell<int>*>(uv1.get());
	//duv1->value=12;
	//std::cout<<*duv1<<*v1<<std::endl;

	//assert(ms2.Rows()==2);
	//assert(ms2.Size()==4);
	//ms2.AddRow();
	//M2M m2ms2(ms2,r1);

	//std::cout<<"m2ms2"<<std::endl;
	//std::cout<<m2ms2<<std::endl;
	//assert(ms2.Rows()==3);
	std::cout<<"END"<<std::endl;
//
    return 0;
}
