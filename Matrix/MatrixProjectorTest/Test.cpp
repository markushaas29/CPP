#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;
using namespace DateTimes;

class MatrixProjectorTest
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
			using QV = Quantity<Volume>;
			using T = std::tuple<Q,QM>;
			using T3 = std::tuple<Q,QM, QV>;
			using T3_2 = std::tuple<QM, QV, Q>;
			using TR = std::tuple<Year,Month,Day,Quantity<Volume>>;
		
			using MIQ2 = MatrixProjector<MI2,Q>;
			using MSQ1 = MatrixProjector<MS1,Q>;
			using MST1 = MatrixProjector<MS1,T>;
			using MIT2 = MatrixProjector<MI2,T>;
			using MIT3 = MatrixProjector<MI2,T3>;
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
		
			MIQ2 miq(m35);
			auto eq7 = miq[1][1];
			assert(Q(7)==eq7);
			assert(Q(13)==miq[2][2]);
			assert(Q(5)==miq[0][4]);
			assert(Q(6)==miq.Slice<0>()[1]);
			assert(Q(15)==miq.Slice<4>()[2]);
  			std::cout<<"\nmit3 Slice "<<miq.Slice<0>()<<std::endl;

			
			MSQ1 msq(ms1);
			auto sq2 = msq[1];
			assert(Q(2)==sq2);
			
			MIT2 mit2(m22);
			auto it2 = mit2[1];
			assert(QM(4)==it2.At<1>());
		
			MST2 mst2(ms22);
			auto st2 = mst2[1];
			assert(QM(6)==st2.At<1>());
			
			MDR2 mr2(md2);
			auto r0 = mr2[0];
			std::cout<<"Reading MST"<<r0.At<1>()<<std::endl;
			assert(QV(5123.9)==r0.At<3>());

			std::cout<<"Reading Q"<<msq<<std::endl;
			std::cout<<"Reading MST"<<mst2<<std::endl;
			std::cout<<"Reading"<<r0.At<1>()<<std::endl;
			std::cout<<"Reading"<<r0.At<0>()<<std::endl;

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

  			auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/Energy.ctrv" };
  			auto mrR = MatrixReader(rpath);
  			auto mR = mrR.M<2>();
  			
  			using QE = Quantity<Work>;
  			using TRF = std::tuple<DateTimes::Date,QE,Entry>;

			auto s0=  m33.Slice(0);
  			using MPS = MatrixProjector<decltype(s0),TRF>;
			MPS mps(s0);

  			using MPR = MatrixProjector<decltype(mR),TRF>;
  			MPR mpr(mR);
  			std::cout<<"ReadLine "<<mpr[1].At<1>()<<std::endl;
  			//std::cout<<"Slice "<<mps[0]<<std::endl;

			MIT3 mit3(m33);
			MIQ2 miq3(m33);
  			std::cout<<"mit3 "<<mit3<<std::endl;
  			std::cout<<"\nmit3 Slice "<<mit3.Slice<0>()<<std::endl;
  			std::cout<<"\nMul "<<mit3[1].multiply(T3_2(2.5,2,3))<<std::endl;
  			std::cout<<"Mul "<<mit3[1].multiply(Q(2.5))<<std::endl;
  			
  			std::cout<<"Mul "<<(mit3[1] * T3(5,7,9))<<std::endl;
  			std::cout<<"Mul "<<(mit3[1] * 3 * 3* 10)<<std::endl;
  			auto rm = mit3[1].multiply(mit3[0]);
  			std::cout<<"Mul Row"<<rm.At<0>()<<std::endl;
  			std::cout<<"Mul Row"<<mit3[1].multiply(mit3[0])<<std::endl;
  			std::cout<<"Mul Row"<<(mit3[1] * mit3.Slice<0>())<<std::endl;
  			std::cout<<"Mul Row"<<(mit3[1] * miq3[2])<<std::endl;

			bool isthrow = false;
			try {mit3[1] * miq3;} catch(...) { isthrow = true; }
			assert(isthrow);


			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
