#include <iostream>
#include "../RepositoryValue.hpp"
#include "../../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    
    std::unique_ptr<FS::RepositoryValue> rv;
    
    rv =  std::make_unique<FS::AccountValue<int>>();
    Logger::Log("Name: ",rv->Name());
    
    rv =  std::make_unique<FS::CounterValue<int>>();
    Logger::Log("Name: ",rv->Name());
    
    return 0;
}
