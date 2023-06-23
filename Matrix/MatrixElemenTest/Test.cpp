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

	auto qe = QuantityElement<Q>("QT");
	std::cout<<vei<<std::endl;

	assert(false);
	
	std::cout<<"END"<<std::endl;
//
    return 0;
}
