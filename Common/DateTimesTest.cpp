#include <iostream>
#include <chrono>
#include <vector>
#include "../Logger/Logger.hpp"
#include "DateTimes.hpp"

using namespace std::chrono;

int main()
{    
	const std::chrono::time_point now{std::chrono::system_clock::now()};
	const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
    
    std::cout << "Current Year: " << static_cast<int>(ymd.year());
    
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
	Logger::Log()<<DateTimes::January.Prev().Value()<<std::endl;
	
	std::cout<<"TODAY"<<DateTimes::Date::Today().Value()<<std::endl;
	std::cout<<"Get"<<DateTimes::Day::Get(1)<<std::endl;
	std::cout<<"Get"<<DateTimes::Month::Get(2)<<std::endl;
	std::cout<<"Get"<<DateTimes::Year::Get(2021)<<std::endl;
	
	std::cout<<"TODAY"<<DateTimes::Date::Today()<<std::endl;
	std::cout<<"TODAY"<<DateTimes::Date(32,15,3001)<<std::endl;
	
	std::cout<<"Extract"<<DateTimes::Date("20.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.1.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.01.2022")<<std::endl;
	
	std::cout<<"Days"<<std::endl;
	
	auto d1 = DateTimes::Date("5.3.2022");
	auto d2 = DateTimes::Date("28.2.2022");
	
	std::cout<<"Days "<<(d2 - d1)<<std::endl;
	std::cout<<"Days "<<(d1 - d2)<<std::endl;

	auto d3 = DateTimes::Date("5.3.2020");
	auto d4 = DateTimes::Date("28.2.2020");
	
	std::cout<<"Days "<<(d3 - d1)<<std::endl;
	std::cout<<"Days "<<(d4 - d3)<<std::endl;
	std::cout<<d4<<" > "<<d3<<"\t"<<(d4 > d3)<<std::endl;
	std::cout<<d4<<"< "<<d3<<"\t"<<(d4 < d3)<<std::endl;
	std::cout<<d4<<"== "<<d3<<"\t"<<(d4 == d3)<<std::endl;
	std::cout<<d3<<"== "<<d3<<"\t"<<(d3 == d3)<<std::endl;
	std::cout<<DateTimes::July<<"== "<<DateTimes::July<<"\t"<<(DateTimes::July == DateTimes::July)<<std::endl;
	auto y2021 = DateTimes::Year(2021);
	auto y2020 = DateTimes::Year(2020);
	std::cout<<"Leap 2020"<<"\t"<<y2020.IsLeap<<std::endl;
	std::cout<<"Leap 2021"<<"\t"<<y2021.IsLeap<<std::endl;
	std::cout<<DateTimes::July<<"== "<<DateTimes::July<<"\t"<<(DateTimes::Year(2021) == DateTimes::Year(2021))<<std::endl;
	
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
