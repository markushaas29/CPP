#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixCell.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
using namespace ShortNames;

int main()
{
	std::cout<<"START"<<std::endl;

	auto vei = ValueElement<IBAN>("IBANT");
	std::cout<<vei<<std::endl;
	auto i = vei.Get();
	bool same = std::is_same_v<decltype(i),IBAN>;
	assert(same);

	auto qe = QuantityElement<Q>("1.53");
	//QuantityElement<Q> qe("1.53");
	std::cout<<qe<<std::endl;
	auto q = qe.Get();
	same = std::is_same_v<decltype(q),Q>;
	assert(same);
	
	std::cout<<"END"<<std::endl;
//
    return 0;
}
