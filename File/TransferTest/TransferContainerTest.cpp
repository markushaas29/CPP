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
	auto y2022 = Y{2022};
	auto y2023 = Y{2023};
	auto q = Q{123};
	auto q2 = Q{124};
	auto d = D();
	auto e = E{"TEST"};
	
	auto tp = std::make_tuple(i,b,n,dt,q,d,e);
	auto t = std::make_shared<TT>(tp);
	auto tp2 = std::make_tuple(i,b,n,dt2,q,d,e);
	auto t2 = std::make_shared<TT>(tp2);
	auto tp3 = std::make_tuple(i,b,n,dt,q2,d,e);
	auto t3 = std::make_shared<TT>(tp3);
	auto tp4 = std::make_tuple(i,b,n,dt2,q2,d,e);
	auto t4 = std::make_shared<TT>(tp4);
	
	auto tc = TC{};
	tc.Add(t);
	tc.Add(t2);
	tc.Add(t3);
	tc.Add(t4);
	std::cout<<(**tc.Begin())<<std::endl;
	assert(tc.Size()==4);
	
	auto f1 = tc.GetTransferOf(i,dt);
	assert(f1->size() == 2);

	auto f2 = tc.GetTransferOf(y2028);
	assert(f2->size() == 0);
	
	auto f3 = tc.GetTransferOf(y2022);
	assert(f3->size() == 2);
	
	auto f4 = tc.GetTransferOf(q);
	assert(f4->size() == 2);
	
	auto f5 = tc.GetTransferOf(q2);
	assert(f5->size() == 2);


    return 0;
}
