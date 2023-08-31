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
			std::cout<<"Add "<<ac(5)<<std::endl;

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

