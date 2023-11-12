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

	ElementVisitor vd;
	std::shared_ptr<IElement> dp = std::make_shared<Date>(29,9,1986);
	std::shared_ptr<IElement> sp = std::make_shared<Quantity<Sum>>(29);
	dp->Accept(vd);
	sp->Accept(vd);

	std::cout<<"END"<<std::endl;
   
	return 0;
}
