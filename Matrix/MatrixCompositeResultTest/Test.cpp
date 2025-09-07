#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixElement.hpp"
#include "../MatrixCompositeResult.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/DateTimes.hpp"
using QS = Quantity<Sum>; 

class MatrixResultTypeTest
{
	public:
	int Run()
{
	std::cout<<"START"<<std::endl;

	std::vector<std::string> s{"1","2","27.12.2022"};
	
	using QS = Quantity<Sum>;
	using QM = Quantity<Mass>;
	using T = std::tuple<QS,QM>;

	auto qm = QM{3};
	auto qs = QS{4};
    std::vector<QS> vqs = {QS{5},QS{5}};
    Acc<QS> acc{vqs};
	std::cout<<"Acc"<<acc<<std::endl;

	Result<QS,Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>> res{QS{5}};

	std::cout<<"END"<<std::endl;
//
    return 0;
}
};

