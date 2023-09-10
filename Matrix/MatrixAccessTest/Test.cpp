#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../M3.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
using namespace ShortNames;
using namespace DateTimes;

class MatrixAccessTest
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
			
			MD2 md2 {
		        {2020, 12, 24, 5123.9},
		        {2019, 11, 23, 5024.9}
		    };
			
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };
	
			bool isthrow = false;
			auto c34 = m35.Cols(3,4);
			assert((int)c34[0][1]==5);
			assert((int)c34[1][0]==9);
			assert((int)c34[2][1]==15);
			try {m35.Cols(9,5);} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;
			
			auto c2 = m35.Col(2);
			assert((int)c2[0]==3);
			assert((int)c2[1]==8);
			try {m35.Col(9);} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;

			auto r12 = m35.Rows(1,2);
			assert((int)r12[0][1]==7);
			assert((int)r12[1][1]==12);
			assert((int)r12[1][3]==14);
			try {m35.Rows(9,10);} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;
			
			auto s12_34 = m35.Sub({1,2},{3,4});
			assert((int)s12_34[0][0]==9);
			assert((int)s12_34[0][1]==10);
			assert((int)s12_34[1][0]==14);
			assert((int)s12_34[1][1]==15);
			try {m35.Sub({1,10},{2,3});} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;
			try {m35.Sub({1,2},{2,7});} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;
			
			auto sl13_24 = m35.Slices({1,2},{2,4});
			assert((int)sl13_24[0][0]==8);
			assert((int)sl13_24[0][1]==10);
			assert((int)sl13_24[1][0]==13);
			assert((int)sl13_24[1][1]==15);
			
			auto sl13_1245 = m35.Slices({1,2},{0,1,3,4});
			assert((int)sl13_1245[0][0]==6);
			assert((int)sl13_1245[1][3]==15);
			assert((int)sl13_1245[1][1]==12);

			try {m35.Slices({4,2},{2,4});} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;
			try {m35.Slices({1,2},{2,7});} catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;

			std::vector<std::vector<std::vector<int>>> v3 = {{{1},{2}},{{3},{4},{5},{6}},{{7},{8},{9},{10},{11},{12}}};
			M3<int> m3(v3);

			assert(m3.Rows()==3);
			try {auto x = m3[4]; } catch(...) { isthrow = true; }
			assert(isthrow);
			isthrow = false;

			std::cout<<m3[1];

			std::cout<<"END"<<m35.Sub({1,2},{2,2})<<std::endl;
		   
			return 0;
		}
};
