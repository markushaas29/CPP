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
			auto b = Var(true);
			assert(b());
			b(false);
			assert(!b());
			assert(Var(true)());
			assert(Var(3!=4)());
			assert(!Var(3==4)());
			assert(!Var(false)());

			auto a = And(std::make_unique<Var>(true),std::make_unique<Var>(true));
			assert(a());
			//std::cout<<"AND "<<a<<std::endl;
			assert(!And(std::make_unique<Var>(true),std::make_unique<Var>(false))());
			assert(!And(std::make_unique<Var>(true),std::make_unique<And>(std::make_unique<Var>(true),std::make_unique<Var>(false)))());

			std::cout<<"END"<<std::endl;

			return 0;
		}
};

