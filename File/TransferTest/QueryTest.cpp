#include <iostream>
#include "../Transfer.hpp"
#include "../AccountQuery.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
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
	
	constexpr auto i = I{"DE3"};
	constexpr auto b = B{"COBA"};
	constexpr auto n = N{"MN"};
	constexpr auto dt = DT{5,3,2022};
	constexpr auto dt2 = DT{6,3,2022};
	constexpr auto q = Q{123};
	constexpr auto q2 = Q{124};
	auto d = D();
	constexpr auto e = E{"TEST"};

	constexpr AccountQuery<A,DT,N,E> aq{i,dt,n,e};
	constexpr AccountQuery<A,DT,E> aq2{i,dt,e};
	//aq.Execute();
	aq.Display(std::cout);

	using AQDTNE = AccountQuery<A,DT,N,E>;
	using AQDTE = AccountQuery<A,DT,E>;
	constexpr AccountQueryContainer<AQDTNE,AQDTE> aqc{aq,aq2};
	aqc.Display(std::cout);
		
    return 0;
}
