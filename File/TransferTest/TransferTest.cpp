#include <iostream>
#include "../Transfer.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
#include "../../Common/ShortNames.hpp"
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
	auto dt = DT{1,1,2000};
	auto q = Q{123};
	auto d = D();
	auto e = E{"TEST"};
	
	auto tp = std::make_tuple(i,b,n,dt,q,d,e);
	auto t = TT{tp};
    return 0;
}
