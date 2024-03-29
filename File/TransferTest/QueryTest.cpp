#include <iostream>
#include "../Transfer.hpp"
#include "../AccountQuery.hpp"
#include "../QueryOperations.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
#include "../../Common/Analyzer.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
using namespace ShortNames;
using namespace Bank;

int main()
{       
    std::cout<<"START"<<std::endl;
	using A = Bank::Raiffeisenbank<Configuration::Raiffeisenbank>;
	using TT = A::TransferType;
	using TP = std::tuple<I, B, N, DT, Q, D, E>;
	
	using AN = Analyzer<A::Base,int>;
        AN::Get();
	
	constexpr auto i = I{"DE3"};
	constexpr auto b = B{"COBA"};
	constexpr auto n = N{"MN"};
	constexpr auto dt = DT{5,3,2022};
	constexpr auto dt2 = DT{6,3,2022};
	constexpr auto q = Q{123};
	constexpr auto q2 = Q{124};
	auto d = D();
	constexpr auto e = E{"TEST"};

	Less l0 = Less{0.0};
	assert(!Less<>::Check(q,0.0));
	//assert(!Less<1.9>::Check(q,0.0));
	assert(!l0(q));

	auto in = Income{};
	assert((q));


	constexpr AccountQuery<A,DT,N,E> aq{i,dt,n,e};
	constexpr ItemQuery<A,DT,N,E> iq{dt,n,e};
	constexpr AccountQuery<A,DT,E> aq2{i,dt,e};
	auto r = aq.Execute();
	auto ri = iq.Execute();
	r.GetSum();
	auto it = r.Items();
	Total<Less<double>>(it->cbegin(),it->cend());
	Total<Income>(r);
	
	r.Display(std::cout);
	aq.Display(std::cout);

	using AQDTNE = AccountQuery<A,DT,N,E>;
	using AQDTE = AccountQuery<A,DT,E>;
	//constexpr AccountQueryContainer<> aqc_e{};
	constexpr AccountQueryContainer<AQDTNE> aqc1{aq};
	constexpr AccountQueryContainer<AQDTNE,AQDTE> aqc2{aq,aq2};
	assert(decltype(aqc1)::QueryCount==1);
	aqc1.Execute();
	aqc1.Display(std::cout);
	
    return 0;
}
