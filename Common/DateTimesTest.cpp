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
	
	auto dy2 = DateTimes::Day{2};
	auto d1 = DateTimes::Date("5.3.2022");
	std::cout<<d1<<" Day: "<<Get<DateTimes::Day>(d1)<<std::endl;
	assert(5==Get<DateTimes::Day>(d1));
	
	auto m1 = Get<DateTimes::Month>(d1);
	std::cout<<d1<<" Month: "<<m1<<std::endl;
	assert(3==m1);
	
	auto y1 = Get<DateTimes::Year>(d1);
	std::cout<<d1<<" Month: "<<y1<<std::endl;
	assert(2022==y1);
	auto d2 = DateTimes::Date("28.2.2022");
	
	std::cout<<"Days "<<(d2 - d1)<<std::endl;
	assert((d2 - d1).Value()==5);
	std::cout<<"Days "<<(d1 - d2)<<std::endl;
	assert((d1 - d2).Value()==5);

	auto d3 = DateTimes::Date("5.3.2020");
	auto d4 = DateTimes::Date("28.2.2020");
	
	std::cout<<"Days "<<(d3 - d1)<<std::endl;
	assert((d3 - d1).Value()==730);
	std::cout<<"Days "<<(d4 - d3)<<std::endl;
	assert((d4 - d3).Value()==6);
	std::cout<<d4<<" > "<<d3<<"\t"<<(d4 > d3)<<std::endl;
	assert(!(d4 > d3));
	std::cout<<d4<<"< "<<d3<<"\t"<<(d4 < d3)<<std::endl;
	assert(d4 < d3);
	std::cout<<d4<<"== "<<d3<<"\t"<<(d4 == d3)<<std::endl;
	assert(d4 != d3);
	std::cout<<d3<<"== "<<d3<<"\t"<<(d3 == d3)<<std::endl;
	assert(d3 == d3);
	std::cout<<DateTimes::July<<"== "<<DateTimes::July<<"\t"<<(DateTimes::July == DateTimes::July)<<std::endl;
	assert(DateTimes::July== DateTimes::July);
	auto y2021 = DateTimes::Year(2021);
	auto y2020 = DateTimes::Year(2020);
	std::cout<<"Leap 2020"<<"\t"<<y2020.IsLeap<<std::endl;
	assert(y2020.IsLeap);
	std::cout<<"Leap 2021"<<"\t"<<y2021.IsLeap<<std::endl;
	assert(!y2021.IsLeap);
	std::cout<<DateTimes::July<<"== "<<DateTimes::July<<"\t"<<(DateTimes::Year(2021) == DateTimes::Year(2021))<<std::endl;
	assert((DateTimes::Year(2021) == DateTimes::Year(2021)));
	
	assert(isYMD(m1));
	assert(isYMD(y1));
	assert(isYMD(dy2));
	auto id = DateTimes::isYMD(0.7);
	assert(!id);
	
	auto cd = (DateTimes::Day)(d1);
	assert(cd.Value() == 5);
	auto cm = (DateTimes::Month)(d1);
	assert(cm.Value() == 3);
	auto cy = (DateTimes::Year)(d1);
	assert(cy.Value() == 2022);
	
	
	auto dy5 = DateTimes::Day{5};
	auto m5 = DateTimes::Month{3};
	auto y2022 = DateTimes::Year{2022};
	assert(y2022 == d1);
	
	std::cout<<"END"<<std::endl;
    
	return 0;
}
