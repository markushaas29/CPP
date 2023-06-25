#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixElement.hpp"
#include "../MatrixTransformer.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/DateTimes.hpp"
using QS = Quantity<Sum>; 
using namespace DateTimes; 

int main()
{
	std::cout<<"START"<<std::endl;


	auto vei = ValueElement<IBAN>("IBANT");
	std::cout<<vei<<std::endl;
	auto i = vei.Get();
	bool same = std::is_same_v<decltype(i),IBAN>;
	assert(same);

	auto qe = QuantityElement<QS>("1.53");
	std::cout<<qe<<std::endl;
	auto q = qe.Get();
	same = std::is_same_v<decltype(q),QS>;
	assert(same);
	
	std::unique_ptr<Element<QS>> ptr = std::make_unique<QS>("");

	auto vd = ValueElement<DateTimes::Date>("1.1.2022");
	auto dp = vd.Ptr();
	auto d = vd.Get();
	std::cout<<d<<std::endl;
	same = std::is_same_v<decltype(d),DateTimes::Date>;
	assert(same);

	auto c = vd.Cast();
	std::cout<<c->Get()<<std::endl;

	using Tuple = std::tuple<Quantity<Sum>,IBAN,DateTimes::Date>;
	auto m = MatrixTransformer<Quantity<Sum>,IBAN,DateTimes::Date>();
	auto t = m.Create();
	std::cout<<std::get<0>(t)<<std::endl;
	std::cout<<std::get<2>(t)<<std::endl;
	std::cout<<"END"<<std::endl;
//
    return 0;
}
