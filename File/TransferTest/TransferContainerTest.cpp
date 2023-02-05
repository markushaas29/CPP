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
	
	auto i = I{"DE83660623660009262008"};
	auto i2 = I{"DE83660623660009262009"};
	auto b = B{"GENODE61DET"};
	auto b2 = B{"GENIDE61DET"};
	auto n = N{"MN"};
	auto n2 = N{"MN2"};
	auto dt = DT{5,3,2022};
	auto dt2 = DT{1,2,2023};
	auto y2028 = Y{2028};
	auto y2022 = Y{2022};
	auto y2023 = Y{2023};
	auto m2 = M{2};
	auto m3 = M{3};
	auto d1 = DY{1};
	auto d5 = DY{5};
	auto q = Q{123};
	auto q2 = Q{124};
	auto d = D();
	auto e = E{"TEST"};
	auto e2 = E{"TEST2"};
	
	auto tp = std::make_tuple(i,b,n,dt,q,d,e);
	auto t = std::make_shared<TT>(tp);
	auto tp2 = std::make_tuple(i,b,n,dt2,q,d,e);
	auto t2 = std::make_shared<TT>(tp2);
	auto tp3 = std::make_tuple(i,b,n,dt,q2,d,e);
	auto t3 = std::make_shared<TT>(tp3);
	auto tp4 = std::make_tuple(i,b,n,dt2,q2,d,e);
	auto t4 = std::make_shared<TT>(tp4);
	
	auto tt_i = std::make_tuple(i2,b,n,dt,q,d,e);
	auto t_i = std::make_shared<TT>(tt_i);
	
	auto tt_b = std::make_tuple(i,b2,n,dt,q,d,e);
	auto t_b = std::make_shared<TT>(tt_b);
	
	auto tt_n = std::make_tuple(i,b,n2,dt,q,d,e);
	auto t_n = std::make_shared<TT>(tt_n);
	
	auto tt_e = std::make_tuple(i,b,n,dt,q,d,e2);
	auto t_e = std::make_shared<TT>(tt_e);
	
	//~ auto tt_ = std::make_tuple(i,b,n,dt,q,d,e);
	//~ auto t_ = std::make_shared<TT>(tt_);
	//~ auto tt_ = std::make_tuple(i,b,n,dt,q,d,e);
	//~ auto t_ = std::make_shared<TT>(tt_);
	//~ auto tt_ = std::make_tuple(i,b,n,dt,q,d,e);
	//~ auto t_ = std::make_shared<TT>(tt_);
	
	auto tc = TC{};
	tc.Add(t);
	tc.Add(t2);
	tc.Add(t3);
	tc.Add(t4);
	tc.Add(t_i);
	tc.Add(t_b);
	tc.Add(t_n);
	tc.Add(t_e);
	std::cout<<(**tc.Begin())<<std::endl;
	assert(tc.Size()==8);
	
	auto f = tc.GetTransferOf(i,dt);
	assert(f->size() == 5);

	f = tc.GetTransferOf(y2028);
	assert(f->size() == 0);
	
	f = tc.GetTransferOf(y2022,y2023);
	assert(f->size() == 0);
	
	f = tc.GetTransferOf(y2022);
	assert(f->size() == 6);
	
	f = tc.GetTransferOf(q);
	assert(f->size() == 6);
	
	f = tc.GetTransferOf(q2);
	assert(f->size() == 2);
	
	f = tc.GetTransferOf(q2,m3,d5);
	assert(f->size() == 1);
	
	f = tc.GetTransferOf(q2,m3,d1);
	assert(f->size() == 0);
	
	f = tc.GetTransferOf(q2,dt2);
	assert(f->size() == 1);
	
	f = tc.GetTransferOf(i2);
	assert(f->size() == 1);
	
	f = tc.GetTransferOf(b2);
	assert(f->size() == 1);

	f = tc.GetTransferOf(n2);
	assert(f->size() == 1);
	
	f = tc.GetTransferOf(e2);
	assert(f->size() == 1);


    return 0;
}
