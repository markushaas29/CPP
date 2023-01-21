#include <iostream>
#include "../Transfer.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/Configuration.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	using A = Bank::Raiffeisenbank<Configuration::Raiffeisenbank>;
	using TT = A::TransferType;
	using TP = std::tuple<IBAN,BIC,Name,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>;
	
	auto tp = std::make_tuple(IBAN{},BIC{}, DateTimes::Date{1,1,2000}, Quantity<Sum>{123}, Bank::Direction(), Entry{});
	
    return 0;
}
