#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixCreator.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;

int main()
{
    using MDS2 = MatrixDescriptor<2,std::string>;
    using MDI2 = MatrixDescriptor<2,int>;
    using MDS1 = MatrixDescriptor<1,std::string>;
    using MDD1 = MatrixDescriptor<1,double>;
    using MS1 = Matrix<1,MDS1>;
    using MD1 = Matrix<1,MDD1>;
    using MS2 = Matrix<2,MDS2>;
    using MI2 = Matrix<2,MDI2>;

	using MIQ2 = MatrixProjector<MI2,Q>;
	std::cout<<"START"<<std::endl;

 	MI2 m35 {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9,10},
        {11, 12, 13, 14, 15},
    };

	MIQ2 miq(m35);

	std::cout<<"END"<<std::endl;
   
	return 0;
}
