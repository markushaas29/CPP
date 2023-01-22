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
	
	auto tp = std::make_tuple(IBAN{},BIC{}, DateTimes::Date{1,1,2000}, Quantity<Sum>{123}, Bank::Direction(), Entry{});
	//~ TP tp1(I{},B{}, DT{1,1,2000}, Q{123}, D(), E{});
	
    return 0;
}
