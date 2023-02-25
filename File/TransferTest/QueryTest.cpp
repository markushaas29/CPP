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
	
	auto i = I{"DE3"};
	auto b = B{"COBA"};
	auto n = N{"MN"};
	auto dt = DT{5,3,2022};
	auto dt2 = DT{6,3,2022};
	auto q = Q{123};
	auto q2 = Q{124};
	auto d = D();
	auto e = E{"TEST"};

	AccountQuery<A,DT> aq{i,dt};
		
    return 0;
}
