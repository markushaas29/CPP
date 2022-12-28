#include <iostream>
#include <vector>
#include "../Logger/Logger.hpp"
#include "DateTimes.hpp"

using namespace std::chrono;

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
	
	std::cout<<"TODAY"<<DateTimes::Date::Today().Value()<<std::endl;
	std::cout<<"TODAY"<<DateTimes::Date::Today()<<std::endl;
	std::cout<<"TODAY"<<DateTimes::Date(32,15,3001)<<std::endl;
	
	std::cout<<"Extract"<<DateTimes::Date("20.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.1.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.01.2022")<<std::endl;
	
	std::cout<<"Days"<<std::endl;
	
	auto d1 = DateTimes::Date("20.11.2022");
	auto d2 = DateTimes::Date("18.11.2022");
	
	auto days = NumberOfDays(d2,d1);
	std::cout<<"Days "<<days<<std::endl;
	
	//~ auto d = Chars(18,12,2021);
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
	//~ std::cout<<d.Value()<<std::endl;
	
	//~ auto dt = DateTimes::Date(18,(uint)12,(uint)2021);
	//~ std::cout<<dt.Value()<<std::endl;
	//~ std::cout<<dt[0]<<std::endl;
	//~ std::cout<<dt[4]<<std::endl;
	//~ std::cout<<dt[9]<<std::endl;
	//~ std::cout<<dt.Size()<<std::endl;
	

	std::cout<<"END"<<std::endl;
    
	return 0;
}
