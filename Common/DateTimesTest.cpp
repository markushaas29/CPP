#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include "../Logger/Logger.hpp"
#include "../Quantity/ToQuantity.hpp"
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
    
	constexpr uint uint2021{2021};
	constexpr auto y2021 = DateTimes::Year::Get(uint2021);
	assert(y2021.Value()==uint2021);
	assert(y2021.ToString()=="2021");


	std::cout<<"TODAY"<<DateTimes::Date::Today().Value()<<std::endl;
	std::cout<<"Get"<<DateTimes::Day::Get(1)<<std::endl;
	std::cout<<"Get"<<DateTimes::Month::Get(2)<<std::endl;
	std::cout<<"Get"<<DateTimes::Year::Get(2021)<<std::endl;
	
	std::cout<<"TODAY"<<DateTimes::Date::Today()<<std::endl;
	std::cout<<"TODAY"<<DateTimes::Date(32,15,3001)<<std::endl;
	
	std::cout<<"Extract"<<DateTimes::Date("20.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.11.2022")<<std::endl;
	std::cout<<"Extract"<<DateTimes::Date("2.1.2022")<<std::endl;
	std::cout<<"Extract 2.01.2022"<<DateTimes::Date("2.01.2022")<<std::endl;
	
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
	
	std::cout<<"Value "<<d3.Value()<<std::endl;
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
	auto y2020 = DateTimes::Year(2020);
	std::cout<<"Leap 2020"<<"\t"<<y2020.IsLeap<<std::endl;
	assert(y2020.IsLeap);
	std::cout<<"Leap 2021"<<"\t"<<y2021.IsLeap<<std::endl;
	assert(!y2021.IsLeap);
	std::cout<<DateTimes::July<<"== "<<DateTimes::July<<"\t"<<(DateTimes::Year(2021) == DateTimes::Year(2021))<<std::endl;
	assert((DateTimes::Year(2021) == DateTimes::Year(2021)));
	
	assert(DateTimes::isYMD<DateTimes::Year>());
	assert(DateTimes::isYMD<DateTimes::Month>());
	assert(DateTimes::isYMD<DateTimes::Day>());
	auto id = DateTimes::isYMD<int>();
	assert(!id);
	
	auto cd = (DateTimes::Day)(d1);
	assert(cd.Value() == 5);
	assert(cd.Valid());
	auto cm = (DateTimes::Month)(d1);
	assert(cm.Value() == 3);
	assert(cd.Valid());
	auto cy = (DateTimes::Year)(d1);
	assert(cy.Value() == 2022);
	assert(cy.Valid());
	
	
	constexpr auto dy5 = DateTimes::Day{5};
	constexpr auto d29 = DateTimes::Day{29};
	constexpr auto m5 = DateTimes::Month{3};
	constexpr auto m2 = DateTimes::Month{2};
	constexpr auto y2022 = DateTimes::Year{2022};
	constexpr auto y2024 = DateTimes::Year{2024};
	assert(y2022 == d1);
	assert(d1 == m5);
	assert(d1 == y2022);

	constexpr auto cd12122021 = DateTimes::Date{12,12,2021};
	constexpr auto cd29022024 = DateTimes::Date{d29,m2,y2024};
	constexpr auto cd29022021 = DateTimes::Date{d29,m2,y2021};
	assert(!cd29022021.Valid());
	assert(cd29022024.Valid());
	constexpr auto date = DateTimes::Date{dy5,m5,y2022}; 

	auto dyi = DateTimes::Day{45};
	auto mi = DateTimes::Month{16};
	auto yi = DateTimes::Year{600};
	assert(!dyi.Valid());
	assert(!mi.Valid());
	assert(!yi.Valid());

	auto dated45 = DateTimes::Date{dyi,m2,y2024};
	auto datem16 = DateTimes::Date{d29,mi,y2024};
	auto datey600 = DateTimes::Date{dy5,m2,yi};
	auto datey25091986 = DateTimes::Date("25.09.1986");
	assert(!dated45.Valid());
	assert(!datem16.Valid());
	assert(!datey600.Valid());
	assert(datey25091986.Valid());
	assert(!DateTimes::Date("").Valid());
	auto dateytest = DateTimes::Date("TEST");
	assert(!dateytest.Valid());

	std::cout<<"TEST 2 "<<DateTimes::Date("TESTEST").Value()<<std::endl;

	auto today = DateTimes::Date::Today();
	//auto tq = ToQuantity<DateTimes::Year>(std::cin, (DateTimes::Year)today);

    std::istringstream is("12 12 1999 13 1 1 2022");

	auto tq1 = ToQuantity<DateTimes::Month>(is,std::cout);
	auto tq2 = ToQuantity<DateTimes::Day>(is,std::cout);
	auto tq3 = ToQuantity<DateTimes::Year>(is,std::cout);
	assert(tq1.Value()==12);
	assert(tq2.Value()==12);
	assert(tq3.Value()==1999);
    
    std::istringstream isp("12.12.1999");
	auto cp = DateTimes::Date::Create(isp);
	
	std::cout<<"\n"<<cp<<std::endl;
	auto mcd = Get<DateTimes::Month>(cp);
	assert(12==mcd);
	auto ycd = Get<DateTimes::Year>(cp);
	assert(1999==ycd);
	
    std::istringstream ise("11 11 1998");
	auto ce = DateTimes::Date::Create(ise);
	
	std::cout<<"\n"<<ce<<std::endl;
	auto mce = Get<DateTimes::Month>(ce);
	assert(11==mce);
	auto yce = Get<DateTimes::Year>(ce);
	assert(1998==yce);
	std::cout<<"\nISP"""<<std::endl;
	

	std::cout<<"TQ m: "<<tq1<<" d "<<tq2<<" "<<tq3<<std::endl;
	DateTimes::Day::Create(is,std::cout);
	auto c = DateTimes::Date::Create(is,std::cout);
	std::cout<<"\n"<<c<<std::endl;
	std::cout<<"END"<<std::endl;

	return 0;
}
