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
	
	using QS = Quantity<Sum>;
	using QM = Quantity<Mass>;
	using T = std::tuple<QS,QM>;

	auto qm = QM{3};
	auto qs = QS{4};

	auto rq = MatrixResultType<QM,QS>::multiply();
	auto si =MatrixResultType<QM,QS>::Size;
	assert(si==1);
	std::cout<<rq<<std::endl;
	auto g = 7;	
	auto t = MatrixResultType<T,QM>::multiply();
	auto t1 = MatrixResultType<decltype(t),QM>::multiply();
	std::cout<<std::get<1>(t1)<<std::endl;
	auto v = std::remove_reference<decltype(std::get<1>(t1).Get())>::type::ValueType(1);
	std::cout<<"GET"<<std::get<1>(t1).Get()<<std::endl;
	std::cout<<"ConstructVal"<<v<<std::endl;
	std::cout<<"ConstructVal: "<<GetValue<0>(rq,g)<<std::endl;
	std::cout<<"ConstructVal: "<<GetValue<1>(t1,g)<<std::endl;
	std::cout<<"T: "<<MatrixResultType<T,QM>::Size<<std::endl;


	decltype(MatrixResultType<T,QM>::multiply()) x;
	std::cout<<std::get<1>(x)<<std::endl;

	std::cout<<"END"<<std::endl;
//
    return 0;
}
