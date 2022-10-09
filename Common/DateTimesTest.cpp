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
	
	
	auto d = DateTimes::Date(18,(uint)12,(uint)2021);
	//~ auto d = DateTimes::Date::getChars(18,12,2021);
	//~ std::cout<<d[0]<<std::endl;
	//~ std::cout<<d[1]<<std::endl;
	//~ std::cout<<d[2]<<std::endl;
	//~ std::cout<<d[3]<<std::endl;
	//~ std::cout<<d[4]<<std::endl;
	//~ std::cout<<d[5]<<std::endl;
	//~ std::cout<<d[6]<<std::endl;
	//~ std::cout<<d[7]<<std::endl;
	//~ std::cout<<std::string(d.data())<<std::endl;
	std::cout<<d.Value()<<std::endl;
	//~ std::cout<<d.Value().size()<<std::endl;
	
	//~ auto day = convertIntegerToChar<1>(1);
	//~ std::cout<<day[0]<<std::endl;
	
	//~ uint i;
	//~ std::cin>>i;
    //~ auto x = DateTimes::Month::Get(i);
	//~ Logger::Log()<<x.Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	return 0;
}
