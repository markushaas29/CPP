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
	auto dt2 = DT{6,3,2022};
	auto q = Q{123};
	auto q2 = Q{124};
	auto d = D();
	auto e = E{"TEST"};
	
	auto tp = std::make_tuple(i,b,n,dt,q,d,e);
	auto t = TT{tp};
	
	auto ti = GetTransfer<I>(t);
	std::cout<<"I: "<<ti<<std::endl;
	assert(ti.Value()==i.Value());
	
	
	auto dy5 = DateTimes::Day{5};
	auto m5 = DateTimes::Month{3};
	auto y2022 = DateTimes::Year{2022};
	assert(t == dy5);
	assert(t == m5);
	assert(t == y2022);
	
	auto dy6 = DateTimes::Day{6};
	auto m6 = DateTimes::Month{6};
	auto y2028 = DateTimes::Year{2028};
	assert(t != dy6);
	assert(m6 != t);
	assert(y2028 != t);
	assert(t == dt);
	assert(dt == (DT)t);
	
	auto ci = static_cast<I>(t);
	std::cout<<"I: "<<ci<<std::endl;
	assert(i == ci);
	
	auto cq = static_cast<Q>(t);
	std::cout<<"Q: "<<cq<<std::endl;
	assert(q == cq);
	
	auto cd = static_cast<double>(t);
	std::cout<<"D: "<<cd<<std::endl;
	assert(q.Value() == cd);
	
	assert(i == t);
	assert(t != dt2);
	assert(t != q2);
		
    return 0;
}
