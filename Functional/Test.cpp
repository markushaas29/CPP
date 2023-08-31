#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Functional.hpp"
#include "../Common/ShortNames.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
using namespace ShortNames;
using namespace DateTimes;

class FunctionalTest
{
	public:
		int Run()
		{
			using QM = Quantity<Mass>;
			using QV = Quantity<Volume>;
			using T = std::tuple<Q,QM>;
			using T3 = std::tuple<Q,QM, QV>;
			using T3_2 = std::tuple<QM, QV, Q>;
			using TR = std::tuple<Year,Month,Day,Quantity<Volume>>;
	
			QM qm{5};
			auto cm = Constant<QM>(qm);
			assert(qm==cm());
			assert(5==(int)cm);
			std::cout<<"Const "<<qm<<std::endl;

			auto ac = Add<Constant<QM>, Constant<QM>>(cm,cm);
			assert(ac()==QM{10});
			assert(10==(int)ac);
			std::cout<<"Add "<<ac(5)<<std::endl;
			
			auto ac2 = Add<decltype(ac), Constant<QM>>(ac,cm);
			assert(ac2()==QM{15});
			assert(15==(int)ac2);
			
			auto sc = Sub<decltype(ac2), Constant<QM>>(ac2,cm);
			assert(sc()==QM{10});
			assert(10==(int)sc);
			
			auto mc = Mul<decltype(ac), Constant<QM>>(ac,cm);
			assert(mc()==(QM{5}*QM{10}));
			assert(50==(int)mc);
			
			auto dc = Div<decltype(mc), Constant<QM>>(mc,cm);
			assert(dc()==QM{10});
			assert(10==(int)dc);
			std::cout<<"Mul "<<mc<<std::endl;
			std::cout<<"Mul "<<mc()<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};

int main()
{
        FunctionalTest analyzer;

        analyzer.Run();

        return 0;
}

