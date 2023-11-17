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
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"

class MatrixParserTest
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
			using ME2D = MatrixDescriptor<2,std::shared_ptr<IElement>>;
		    using M2E = Matrix<2,ME2D>;
		
			std::cout<<"START"<<std::endl;
		
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };

			M2E m2e {                
				{std::make_shared<Quantity<Sum>>(2.5), std::make_shared<Quantity<Sum>>(3.5)},
				{std::make_shared<Quantity<Sum>>(4.5), std::make_shared<Quantity<Sum>>(3.5)},
			};	

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
