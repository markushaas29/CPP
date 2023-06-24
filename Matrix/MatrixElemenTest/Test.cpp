#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixCell.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
using QS = Quantity<Sum>; 

int main()
{
	std::cout<<"START"<<std::endl;


	auto vei = ValueElement<IBAN>("IBANT");
	std::cout<<vei<<std::endl;
	auto i = vei.Get();
	bool same = std::is_same_v<decltype(i),IBAN>;
	assert(same);

	auto qe = QuantityElement<QS>("1.53");
	//QuantityElement<Q> qe("1.53");
	std::cout<<qe<<std::endl;
	auto q = qe.Get();
	same = std::is_same_v<decltype(q),QS>;
	assert(same);
	
	std::unique_ptr<Element<QS>> ptr = std::make_unique<QS>("");

	std::cout<<"END"<<std::endl;
//
    return 0;
}
