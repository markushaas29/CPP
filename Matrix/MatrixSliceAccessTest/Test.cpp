#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"

class MatrixColAccessTest
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
			using Q = Quantity<Sum>;
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

			assert(ms1.ColSum(0) == 3);
		
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
	
			auto s024 = m35.Cols(0,2,4);
			std::cout<<s024<<std::endl;
			std::cout<<m33<<std::endl;
			auto mulS = s024 * m33;
			std::cout<<mulS[0]<<std::endl;
			assert((int)mulS[2][2]==410);
			std::cout<<mulS.Cols(1,2)<<std::endl;
			assert((int)mulS[0][0]==92);

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
