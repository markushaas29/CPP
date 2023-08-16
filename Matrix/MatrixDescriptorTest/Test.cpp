#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixCreator.hpp"
#include "../MatrixDescriptor.hpp"
#include "../Matrix_Impl.hpp"
#include "../../Common/ShortNames.hpp"
using namespace ShortNames;
class MatrixDescriptorTest
{
	public:
		int Run()
		{
			try
			{
				std::cout<<"START"<<std::endl;
		
				using MD3 = MatrixDescriptor<3,double>;
				using MD2 = MatrixDescriptor<2,std::string>;
				using MD1 = MatrixDescriptor<1,std::string>;
		
				using MI3 = MatrixImpl<3,MD3>;
				using MI2 = MatrixImpl<2,MD2>;
				using MI1 = MatrixImpl<1,MD1>;
		
				std::array<size_t,1> a1{2};
				std::array<size_t,2> a2{2,3};
				std::array<size_t,2> a2E{1,2};
				std::array<size_t,3> a3{1,2,3};
		
				MD1 md1(a1);
				assert(md1.Rows()==2);
		
				MD2 md2{a2};
				assert(md2.Cols()==3);
				
				MD2 ms254{{5,4}};
				assert(ms254.Rows()==5);
				assert(ms254.Cols()==4);
				assert(ms254.Size()==20);
		
				bool t = false;
				try	{	MD2 md2{{1,0}};	}
				catch(...){	t = true; }
				assert(t);
				
				std::cout<<"END"<<std::endl;
		//
		    	return 0;
			}
			catch(...)
			{
				throw;
			}
		}

};
