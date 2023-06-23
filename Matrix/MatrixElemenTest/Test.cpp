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

	auto qe = QuantityElement<Q>("1.53");
	std::cout<<qe<<std::endl;

	assert(false);
	
	std::cout<<"END"<<std::endl;
//
    return 0;
}
