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

			auto pv = std::make_shared<Var>(true);
			auto a = And(std::make_shared<Var>(true),pv);
			assert(a());
			(*pv)(false);
			assert(!a());
			assert(!And(std::make_shared<Var>(true),pv)());
			(*pv)(true);
			assert(And(std::make_shared<Var>(true),pv)());
			
			assert(!And(std::make_shared<Var>(true),std::make_shared<And>(std::make_shared<Var>(true),std::make_shared<Var>(false)))());

			std::cout<<"END"<<std::endl;

			return 0;
		}
};

