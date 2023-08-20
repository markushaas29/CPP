#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;
using namespace DateTimes;

class MatrixSliceAccessTest
{
	public:
		int Run()
		{
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MDI2 = MatrixDescriptor<2,int>;
		    using MDD2 = MatrixDescriptor<2,double>;
		    using MDS1 = MatrixDescriptor<1,std::string>;
		    using MDD1 = MatrixDescriptor<1,double>;
		    using MS1 = Matrix<1,MDS1>;
		    using MD1 = Matrix<1,MDD1>;
		    using MD2 = Matrix<2,MDD2>;
		    using MS2 = Matrix<2,MDS2>;
		    using MI2 = Matrix<2,MDI2>;
		
			using QM = Quantity<Mass>;
			using T = std::tuple<Q,QM>;
			using TR = std::tuple<Year,Month,Day,Quantity<Volume>>;
		
			using MIQ2 = MatrixProjector<MI2,Q>;
			using MSQ1 = MatrixProjector<MS1,Q>;
			using MST1 = MatrixProjector<MS1,T>;
			using MIT2 = MatrixProjector<MI2,T>;
			using MST2 = MatrixProjector<MS2,T>;
			using MDR2 = MatrixProjector<MD2,TR>;
			
			using MDF2 = MatrixFilter<MD2>;
			using MIF2 = MatrixFilter<MI2>;
			using MSF2 = MatrixFilter<MS2>;
			std::cout<<"START"<<std::endl;
		
			MS1 ms1{
				{std::string("1"),std::string("2")} 
		    };
		
		 	MI2 m35 {
		        {1, 2, 3, 4, 5},
		        {6, 7, 8, 9,10},
		        {11, 12, 13, 14, 15},
		    };
		 	
			MI2 m33 {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };
		 	
			MI2 m22 {
		        {1, 2},
				{3, 4}
		    };
			
			MD2 md2 {
		        {2020, 12, 24, 5123.9},
		        {2019, 11, 23, 5024.9}
		    };
			
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };
		
			MIF2 mif2(m35);
			auto m35F = mif2(2,[&](int d) { return d > 5;});
			assert(m35F.Rows()==2);
			assert((int)m35F[0][2]>5);
			assert((int)m35F[1][2]>5);
			
			MDF2 mdf2(md2);
			auto md5100 = mdf2(0,[&](double d) { return d == 2020; });
			assert(md5100.Rows()==1);
			assert((int)md5100[0][1]==12);
			assert((int)md5100[0][2]==24);
			assert((double)md5100[0][3]==5123.9);
			
			MSF2 msf2(ms22);
			auto ms5 = msf2([&](const auto s) { return *s[0] == "5"; });
			assert(ms5.Rows()==1);
			assert((std::string)ms5[0][0]=="5");
			assert(ms5[0][0].To<int>()==5);
			std::cout<<"\nFilter"<<ms5[0][0].To<int>()<<std::endl;
			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
