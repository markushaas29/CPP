#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixElement.hpp"
#include "../MatrixResultType.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/DateTimes.hpp"
using QS = Quantity<Sum>; 
using namespace DateTimes; 

int main()
{
	std::cout<<"START"<<std::endl;

	std::vector<std::string> s{"1","2","27.12.2022"};
	using T = std::tuple<Quantity<Sum>,IBAN,DateTimes::Date>;
	
	using QS = Quantity<Sum>;
	using QM = Quantity<Mass>;

	auto qm = QM{3};
	auto qs = QS{4};

	auto rq = MatrixResultType<QM,QS>::create();
	std::cout<<rq<<std::endl;
	
	auto t = MatrixResultType<QM,T>::create();
	std::cout<<t<<std::endl;

	std::cout<<"END"<<std::endl;
//
    return 0;
}
