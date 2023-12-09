#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Visitor.hpp"
#include "ElementVisitor.hpp"
#include "../CSV/Elements.hpp"
#include "../Common/Date.hpp"
#include "../Quantity/Quantity.hpp"

class VisitorTest{
	public:
int Run()
{
	std::cout<<"START"<<std::endl;

	ElementVisitor vd;
	std::shared_ptr<IElement> dp = std::make_shared<Date>(29,9,1986);
	std::shared_ptr<IElement> sp = std::make_shared<Quantity<Sum>>(29);
	auto qs = Quantity<Sum>(29);
	IElement* ie = &qs;
	dp->Accept(vd);
	sp->Accept(vd);
	ie->Accept(vd);

	const auto cd = Date(1,1,2020);
	cd.Accept(vd);

	auto pv = PredicateVisitor(std::make_unique<Quantity<Sum>>(29));
	assert(sp->Is(pv));
	assert(sp->Is(pv));
	auto pv228 = PredicateVisitor(std::make_unique<Quantity<Sum>>(228));
	assert(!sp->Is(pv228));

	std::cout<<"END"<<std::endl;
   
	return 0;
}
};
