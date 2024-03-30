#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixFormatter.hpp"
#include "../../CSV/Elements.hpp"

class MatrixFormatterTest
{
	public:
		int Run()
		{
			std::cout<<"START"<<std::endl;

			using Q = Quantity<Sum>;
			using M3 = Matrix<3>;
			using M2 = Matrix<2>;
			using M1 = Matrix<1>;
			using M3DS = MatrixDescriptor<3,double>;
			using M2DS = MatrixDescriptor<2,double>;
			using M1DS = MatrixDescriptor<1,double>;
			using M1PS = MatrixDescriptor<1,Parameter<int>>;
			using M2PS = MatrixDescriptor<2,Parameter<int>>;
			using MS1 = MatrixDescriptor<1,std::string>;
			using MS2 = MatrixDescriptor<2,std::string>;
			using M1S = Matrix<1,MS1>;
			using M2S = Matrix<2,MS2>;
			using M3D = Matrix<3,M3DS>;
			using M2D = Matrix<2,M2DS>;
			using M2P = Matrix<2,M2PS>;
			using M1D = Matrix<1,M1DS>;
			using QS = Quantity<Scalar,Pure,double>;
		
			M1S m1s{
				{std::string("31.10.1986"),std::string("29.09.1986"),std::string("27.03.1986")}	
			};

			M1S m1S{
				{std::string("4"),std::string( "5"),std::string( "6")}
			};
			M1 m13{
				{5,3,1}	
			};
			M1 m1{
				{1,2,3,4,5}	
			};
			
			M2 m35 {
				{1, 2, 3, 4, 5},
				{6, 7, 8, 9,10},
				{11, 12, 13, 14, 15},
			};
			
			M2 m33
			{
				{1, 2, 3},
				{4, 5, 6},
				{7, 8, 9}
			};
			
			M2S m33S
			{
				{"1", "2", "3"},
				{"4", "5", "6"},
				{"7", "8", "9"}
			};
		
			M2 m44 
			{
				{1, 2, 3, 4},
				{6, 7, 8, 9},
				{10, 12, 13, 14},
				{16, 17, 18, 19}
			};
			
			M2D m2d {
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 4.4},
				{1.1, 2.2, 3.3, 5.4}
			};
		
		
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
		
	      	M1D hmc{
	              {(104.5 * 1.19), (143 * 1.19), (450 *1.19)} 
	      	};
	
	      	M2D hms {
	              {1, 2, 1},
	              {1, 1, 0},
	              {1, 1, 0}
	      	};
	
			auto mf1 = MatrixFormatter(m1);
			std::cout<<"END Formatter"<<mf1<<std::endl;
		//   
			return 0;
		}
};