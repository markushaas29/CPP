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
			auto b = Var<bool>(true);
			assert(b());
			assert(b==b);
			b(false);
			assert(!b());
			assert(Var<bool>(true)());
			assert(Var<bool>(3!=4)());
			assert(!Var<bool>(3==4)());
			assert(!Var<bool>(false)());

			auto pv = std::make_shared<Var<bool>>(true);
			std::shared_ptr<IExpression<bool>> pv2 = pv->Clone();
			assert((*pv)());
			assert((*pv2)());
			//assert(pv2==pv);
			auto a = And(std::make_shared<Var<bool>>(true),pv);
			assert(a());
			(*pv)(false);
			assert(!a());
			assert(!And(std::make_shared<Var<bool>>(true),pv)());
			(*pv)(true);
			assert(And(std::make_shared<Var<bool>>(true),pv)());
			
			assert(!And(std::make_shared<Var<bool>>(true),std::make_shared<And>(std::make_shared<Var<bool>>(true),std::make_shared<Var<bool>>(false)))());
			auto ao = std::make_shared<And>(std::make_shared<Var<bool>>(true),std::make_shared<And>(std::make_shared<Var<bool>>(true),std::make_shared<Var<bool>>(false)));

			std::cout<<*ao<<std::endl;

			std::cout<<"END"<<std::endl;

			return 0;
		}
};

