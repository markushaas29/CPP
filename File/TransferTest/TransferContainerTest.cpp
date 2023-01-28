#include <iostream>
#include "../Transfer.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	using A = Bank::Raiffeisenbank<Configuration::Raiffeisenbank>;
	using TT = A::TransferType;
	using TP = std::tuple<I, B, N, DT, Q, D, E>;
	
	auto i = I{"DE3"};
	auto b = B{"COBA"};
	auto n = N{"MN"};
	auto dt = DT{5,3,2022};
	auto dt2 = DT{1,2,2023};
	auto y2028 = Y{2028};
	auto q = Q{123};
	auto d = D();
	auto e = E{"TEST"};
	
	auto tp = std::make_tuple(i,b,n,dt,q,d,e);
	auto t = std::make_shared<TT>(tp);
	
	auto tc = TC{};
	tc.Add(t);
	std::cout<<(**tc.Begin())<<std::endl;
	assert(tc.Size()==1);
	
	auto f1 = tc.GetTransferOf(i,dt);
	assert(f1->size() == 1);

	auto f2 = tc.GetTransferOf(y2028);
	assert(f2->size() == 0);
	//~ std::cout<<(*());
    return 0;
}
