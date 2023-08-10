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
	using MSQ1 = MatrixProjector<MS1,Q>;
	std::cout<<"START"<<std::endl;

	MS1 ms1{
		{std::string("1"),std::string("2")} 
    };

 	MI2 m35 {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9,10},
        {11, 12, 13, 14, 15},
    };

	MIQ2 miq(m35);
	auto eq7 = miq[1][1];
	assert(Q(7)==eq7);
	
	MSQ1 msq(ms1);
	auto sq2 = msq[1];
	assert(Q(2)==sq2);

	std::cout<<"E"<<sq2<<std::endl;
	std::cout<<"END"<<std::endl;
   
	return 0;
}
