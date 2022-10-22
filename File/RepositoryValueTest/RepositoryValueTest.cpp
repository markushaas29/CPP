#include <iostream>
#include "../RepositoryValue.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Common/UniqueCast.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    
    std::unique_ptr<FS::RepositoryValue> rv;
    
    rv =  std::make_unique<FS::AccountValue<int>>();
    Logger::Log("Name: ",rv->Name());
        
	auto av = Cast::static_unique_ptr<FS::AccountValue<int>>(std::move(rv));    
    Logger::Log("Val: ",av->Val());
	
    rv =  std::make_unique<FS::CounterValue<int>>();
    Logger::Log("Name: ",rv->Name());
	
	auto cv = Cast::static_unique_ptr<FS::CounterValue<int>>(std::move(rv));    
    Logger::Log("Val: ",cv->Val1());
    
    return 0;
}
