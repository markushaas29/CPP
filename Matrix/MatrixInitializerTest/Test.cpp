#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixCreator.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixInitializer.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;

class MatrixInitializerTest
{
	public:
		int Run()
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
		
			std::vector<int> v1{1,2,3,4};
			std::vector<std::vector<int>> v2{{1,2,3},{4,5,6}};
			std::vector<std::vector<std::vector<int>>> v3{{{1},{2}},{{3},{4}}};
			std::vector<std::vector<std::vector<int>>> v33{{{1,1,1},{2,2,2}},{{3,3,3},{4,4,4}},{{3,3,3},{4,4,4}}};
		
			auto mi1 = Init(v1);
			auto mi2 = Init(v2);
			auto mi3 = Init(v3);
			auto mi33 = Init(v33);
		
			assert(decltype(mi1)::Order==1);
			assert(decltype(mi2)::Order==2);
			assert(decltype(mi3)::Order==3);
			assert(mi1.Descriptor().Rows()==4);
			assert(mi2.Descriptor().Rows()==2);
			assert(mi3.Descriptor().Rows()==2);
			assert(mi33.Descriptor().Rows()==3);
			assert(mi2.Descriptor().Cols()==3);
			assert(mi3.Descriptor().Cols()==2);
			assert(mi33.Descriptor().Cols()==2);
			assert(mi1.Descriptor().Extents()[0]==4);
			assert(mi2.Descriptor().Extents()[1]==3);
			assert(mi3.Descriptor().Extents()[2]==1);
			assert(mi1.Valid());
			assert(mi2.Valid());
			assert(mi3.Valid());
			assert(mi33.Valid());
		
		  	std::cout<<"mi1: "<<mi1<<std::endl;
		  	std::cout<<"mi2: "<<mi2<<std::endl;
		  	std::cout<<"mi3: "<<mi3<<std::endl;
		  	std::cout<<"mi3: "<<mi33<<std::endl;
		
		  	std::cout<<"MI2: "<<mi2.Get()<<std::endl;
			auto path = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/D.mat" };
			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto sem = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/Sem.mat" };
			auto spath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/S.csv" };
			auto ipath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/I.mat" };
			auto uipath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/UI.mat" };
			auto m22r = MatrixReader(u22);
			auto msr = MatrixReader(path);
			auto mssr = MatrixReader(spath);
			//auto msem = MatrixReader<double>(ipath);
			auto mdr = MatrixReader<double>(path);
			auto muir = MatrixReader<uint>(uipath);
			auto mir = MatrixReader<int>(ipath);
			
			assert(mdr.IsDim<2>());
			auto md2 = mdr.M<2>();
			assert(md2.Rows()==2);
			assert(md2.Cols()==4);
			
			assert(msr.IsDim<2>());
			auto ms2 = msr.M<2>();
			assert(ms2.Rows()==2);
			assert(ms2.Cols()==4);
			
			assert(muir.IsDim<1>());
			auto mui2 = muir.M<1>();
			assert(mui2.Rows()==5);
			assert(mui2.Cols()==1);
			
			assert(mir.IsDim<1>());
			auto mri1 = mir.M<1>();
			assert(mri1.Rows()==5);
			assert(mri1.Cols()==1);
			//
			//auto mir = MatrixReader<int>(ipath);
			//std::cout<<"MI: "<<mir.M<1>()<<std::endl;
			//std::cout<<"Sem: "<<msem.M<1>()<<std::endl;
			//assert(mir.IsDim<1>());
			std::cout<<"MS"<<mssr.M<2>()<<std::endl;
		
			auto msm = mssr.M<2>();
			assert(mssr.IsDim<2>());
			std::cout<<"M"<<msm[3][5]<<std::endl;
			//std::cout<<"M"<<(msm[3][5].To<uint>)<<std::endl;
		//	std::cout<<"MS 22"<<m22r.M<2>()<<std::endl;
		
			std::cout<<"END"<<decltype(mi3)::Order<<std::endl;

			return 0;
		}
};
