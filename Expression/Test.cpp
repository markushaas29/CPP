#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Expression.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"

class ExpressionTest
{
	public:
		int Run()
		{
			std::cout<<"START"<<std::endl;
			using QM = Quantity<Mass>;
			using QV = Quantity<Volume>;
			using T = std::tuple<Quantity<Sum>,Quantity<Mass>>;
	
			QM qm{5};

			assert(BooleanExpression(true)());
			assert(BooleanExpression(3!=4)());
			assert(!BooleanExpression(3==4)());
			assert(!BooleanExpression(false)());

			auto a = And(std::make_unique<BooleanExpression>(true),std::make_unique<BooleanExpression>(true));
			assert(a());
			//std::cout<<"AND "<<a<<std::endl;
			assert(!And(std::make_unique<BooleanExpression>(true),std::make_unique<BooleanExpression>(false))());

			std::cout<<"END"<<std::endl;

			return 0;
		}
};

