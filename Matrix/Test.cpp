#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Matrix.hpp"
#include "MatrixCreator.hpp"
#include "MatrixDescriptor.hpp"
#include "../Common/ShortNames.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	using MS3 = MatrixDescriptor<3,double>;
	using MS2 = MatrixDescriptor<2,std::string,std::string>;
	using MS1 = MatrixDescriptor<1,std::string,std::string>;
	using MQ1DS = MatrixDescriptor<1,double,Quantity<Sum>>;
	using M3D = Matrix<3,MS3>;
	using M1S = Matrix<1,MS1>;
	using M1Q = Matrix<1,MQ1DS>;
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
	assert(m1.Rows()==5);
	assert(m1.Cols()==1);
	auto m1a = m1;
	//auto e1 = m1.ElementsAt(4);
	//assert(e1[0].Get()==5);
	//auto e1a = m1a.ElementsAt(4);
	//e1a = m1a.ElementsAt(4);
	//auto e1a1 = m1a.ElementAt(4);
	
	M1S m1s{
		{std::string("1"),std::string("2")}	
	};
	assert(m1s.Rows()==2);
	assert(m1s.Cols()==1);
	assert(m1s[1]()=="2");
    auto m1sc(m1s);
	assert(m1sc.Rows()==2);
	assert(m1sc.Cols()==1);
	//assert(m1sc.ElementsAt(1)[0].Get()=="2");
    auto m1sa = m1sc;
	assert(m1sa.Rows()==2);
	assert(m1sa.Cols()==1);
	//assert(m1sa.ElementsAt(1)[0].Get()=="2");
	//auto isEntry = std::is_same_v<decltype(m1s.ElementsAt(1)[0].Get()),Entry>;
	//assert(isEntry);
	
	std::vector<std::string> v1s{"3"};
	m1s.AddRow(v1s);
	assert(m1s.Rows()==3);
	assert(*m1s(2)=="3");
	
	M1Q m1q{
		{1.0,2.2,3.3,4.4,5.5}	
	};
	auto qm1 = m1q[4];
	assert((double)qm1==5.5);
	std::cout<<"Matrix m1Q "<<qm1()<<"\n";
	std::vector<std::shared_ptr<double>> v1d{std::make_shared<double>(1.1)};
	auto mqd = MQ1DS{1};
	auto m1qv = M1Q(mqd,v1d);
	qm1 = m1qv[0];
	std::cout<<"Matrix m1Q "<<qm1<<"\n";

	M2 m2
	{
		{1},
		{2},
	};
	assert(m2.Rows()==2);
	assert(m2.Cols()==1);
	assert(m2.Size()==2);
	auto m20 = m2[0];
	assert(m20.Size()==1);
	auto r11 = m2.Row(0);
	assert(*r11[0]==1);
	//m2 * m2;
	M2 m35 {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9,10},
		{11, 12, 13, 14, 15},
	};
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
	auto d = m3[0];//.Descriptor();
	auto dd = d[1];//.Descriptor();
	//auto ddd = dd[1];//.Descriptor();
	std::cout<<"Matrix Element Rows"<<d.Rows()<<"\n";
	std::cout<<"Matrix 3\n"<<m3<<"\n";
	//std::cout<<"Matrix 3 El"<<ddd<<"\n";
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
  	//auto el = mc2m.ElementAt(15,5);
	//isEntry = std::is_same_v<decltype(el)::Type,std::string>;
	//assert(isEntry);
	//auto e15 = mc2m.ElementsAt(15);
	assert(mc2m.Rows()==125);
	assert(mc2m.Cols()==19);
	std::cout<<"END"<<std::endl;
   
	return 0;
}
