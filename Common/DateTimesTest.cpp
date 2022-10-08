#include <iostream>
#include <vector>
#include "../Logger/Logger.hpp"
#include "DateTimes.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    auto j = DateTimes::Month::Get(1);
	Logger::Log()<<j.Value()<<std::endl;
	Logger::Log()<<j.Next().Value()<<std::endl;
	Logger::Log()<<j.Prev().Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	auto y = DateTimes::Year::Get(2021);
	Logger::Log()<<y.Value()<<std::endl;
	Logger::Log()<<y.Prev().Value()<<std::endl;
	
	Logger::Log()<<DateTimes::July.Value()<<std::endl;
	Logger::Log()<<DateTimes::July.Prev().Value()<<std::endl;
	
	Logger::Log()<<DateTimes::January.Value()<<std::endl;
	Logger::Log()<<DateTimes::January.Prev().Value()<<std::endl;
	
	//~ uint i;
	//~ std::cin>>i;
    //~ auto x = DateTimes::Month::Get(i);
	//~ Logger::Log()<<x.Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	return 0;
}
