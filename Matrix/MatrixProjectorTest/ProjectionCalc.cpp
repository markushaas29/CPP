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
#include "../MatrixAnalyzer.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
using namespace ShortNames;
using namespace DateTimes;

class MatrixProjectorCalculationTest
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
		
			using QV = Quantity<Volume>;
			using QM = Quantity<Mass>;
			using QS = Quantity<Sum>;
			using QSC = Quantity<Scalar>;
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
			using MSC2 = MatrixProjector<MS2,QSC>;
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
			
			MD2 m33D {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };
		 	
			MI2 m22 {
		        {1, 2},
				{3, 4}
		    };
			
			auto md22 = m22.To<double>();
			assert((double)md22[1][0]==3.0);
			assert((double)md22[1][1]==4.0);
			
			auto mst22 = m22.To<std::string>();
			std::cout<<"To String:\n"<<(mst22)<<std::endl;
			assert((std::string)mst22[1][0]=="3");
			
			MD2 md2 {
		        {2020, 12, 24, 5123.9},
		        {2019, 11, 23, 5024.9}
		    };
			
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };
  			
			auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN.csv" };
  			auto mrR = MatrixReader(rpath);
  			auto mR = mrR.M<2>().Cols(3,4,5,6,7);
			auto mdR = mR.To<double>();
			assert((int)mdR[1][0]==3);
			assert((int)mdR[0][1]==458);

			auto cs = mdR / mR.ColSum();

			std::cout<<"Stages:\n"<<(cs)<<std::endl;
  			
  			using MPSC = MatrixProjector<decltype(mR),QSC>;
			MPSC mps(mR);
			std::cout<<"Stages:\n"<<mps<<std::endl;
  			
			using QE = Quantity<Work>;
  			using TRF = std::tuple<DateTimes::Date,QE,Entry>;

			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);

			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();

			auto a22 = MatrixAnalyzer<decltype(m22S)>(m22S);
			auto a23 = MatrixAnalyzer<decltype(m23S)>(m23S);
			a22();
			auto out23M = a23();

			auto out23S = out23M.Col(2).To<QS>();
			auto stagS = cs.To<QSC>();
			std::cout<<out23S<<std::endl;
			std::cout<<stagS<<std::endl;

			auto sums = stagS*out23S.Rows(1,2,3,4,5);
			std::cout<<sums[0]<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
