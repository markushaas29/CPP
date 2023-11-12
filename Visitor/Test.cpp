#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Visitor.hpp"
#include "../CSV/Elements.hpp"
#include "../Common/Date.hpp"
#include "../Quantity/Quantity.hpp"

int main()
{
	std::cout<<"START"<<std::endl;

	ElementVisitor<Date> vd;
	Date d1(29,9,1986);

	std::cout<<"END"<<std::endl;
   
	return 0;
}
