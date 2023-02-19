#include <iostream>
#include "../Transfer.hpp"
#include "../Node.hpp"
#include "../Raiffeisenbank.hpp"
#include "../../Common/Configuration.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	
	auto keys = FS::ReadLines("/home/markus/Downloads/CSV_Files/Raiba.keys");
	TI::Instance().UpdateKeyIndices(keys.cbegin(), keys.end());
	
    return 0;
}
