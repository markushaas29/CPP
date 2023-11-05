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
    auto j = Month::Get(1);
	Logger::Log()<<j.Value()<<std::endl;
	Logger::Log()<<j.Next().Value()<<std::endl;
	Logger::Log()<<j.Prev().Value()<<std::endl;
	std::cout<<"END"<<std::endl;
    
	constexpr uint uint2021{2021};
	constexpr auto y2021 = Year::Get(uint2021);
	assert(y2021.Value()==uint2021);
	assert(y2021.ToString()=="2021");


	std::cout<<"Get"<<Day::Get(1)<<std::endl;
	std::cout<<"Get"<<Month::Get(2)<<std::endl;
	std::cout<<"Get"<<Year::Get(2021)<<std::endl;
	
	std::cout<<"TODAY"<<Date::Today()<<std::endl;
	std::cout<<"TODAY"<<Date(32,15,3001)<<std::endl;
	
	std::cout<<"Extract"<<Date("20.11.2022")<<std::endl;
	std::cout<<"Extract"<<Date("2.11.2022")<<std::endl;
	std::cout<<"Extract"<<Date("2.1.2022")<<std::endl;
	std::cout<<"Extract 2.01.2022"<<Date("2.01.2022")<<std::endl;
	
	std::cout<<"Days"<<std::endl;
	
	auto dy2 = Day{2};
	assert(dy2==2);
	auto dy3 = Day{3};
	assert(dy3==3);
	dy2 = dy3;
	assert(dy2==3);
	auto d1 = Date("5.3.2022");
	assert(5==d1.D());
	assert(3==d1.M());
	assert(2022==d1.Y());
	auto d2 = Date("28.2.2022");
	
	std::cout<<"Days "<<(d2 - d1)<<std::endl;
	assert((d2 - d1).Value()==5);
	std::cout<<"Days "<<(d1 - d2)<<std::endl;
	assert((d1 - d2).Value()==5);

	auto d3 = Date("5.3.2020");
	auto d4 = Date("28.2.2020");
	auto d5 = Date("15.5.2023");
	d1 = d4;
	assert(28==d1.D());
	assert(2==d1.M());
	assert(2020==d1.Y());
	d4 = d5;
	assert(15==d4.D());
	assert(5==d4.M());
	assert(2023==d4.Y());
	assert(28==d1.D());
	assert(2==d1.M());
	assert(2020==d1.Y());
	d4 = Date("28.2.2020");
	auto d3Mind4 = d3-d4;
	assert(d3Mind4.Value()==6);
	
	d1 = Date("5.3.2022");
	assert((d3 - d1).Value()==730);
	d4 = Date("28.2.2020");
	assert((d4 - d3).Value()==6);
	assert(!(d4 > d3));
	assert(d4 < d3);
	assert(d4 != d3);
	assert(d3 == d3);
	assert(Jul== Jul);
	assert(::May==::May);
	auto y2020 = Year(2020);
	assert(y2020.IsLeap);
	assert(!y2021.IsLeap);
	assert((Year(2021) == Year(2021)));
	
	assert(isYMD<Year>());
	assert(isYMD<Month>());
	assert(isYMD<Day>());
	auto id = isYMD<int>();
	assert(!id);
	
	auto cd = (Day)(d1);
	assert(cd.Value() == 5);
	auto cd2 = cd;
	assert(cd2.Value() == 5);
	assert(cd.Valid());
	auto cm = (Month)(d1);
	assert(cm.Value() == 3);
	assert(cd.Valid());
	auto cy = (Year)(d1);
	assert(cy.Value() == 2022);
	assert(cy.Valid());
	
	constexpr auto dy5 = Day{5};
	constexpr auto d29 = Day{29};
	constexpr auto m5 = Month{3};
	constexpr auto m2 = Month{2};
	constexpr auto m3 = Month{3};
	constexpr auto y2022 = Year{2022};
	constexpr auto y2024 = Year{2024};
	assert(y2022 == d1);
	assert(d1 == m3);
	assert(d1 == y2022);

	auto cd12122021 = Date{12,12,2021};
	auto cd29022024 = Date{d29,m2,y2024};
	auto cd29022021 = Date{d29,m2,y2021};
	assert(!cd29022021.Valid());
	assert(cd29022024.Valid());
	auto date = Date{dy5,m5,y2022}; 

	auto dyi = Day{45};
	auto mi = Month{16};
	auto yi = Year{600};
	assert(!dyi.Valid());
	assert(!mi.Valid());
	assert(!yi.Valid());
	
	auto date31101986 = Date("31.10.1986");
	assert(date31101986.Valid());
	assert(date31101986.D()==31);
	assert(date31101986.M()==10);
	assert(date31101986.Y()==1986);
    
	Date d31101986;
	std::istringstream is31("31.10.1986");
	is31>>d31101986;
	assert(d31101986.Valid());
	assert(d31101986.D()==31);
	assert(d31101986.M()==10);
	assert(d31101986.Y()==1986);

	auto dated45 = Date{dyi,m2,y2024};
	auto datem16 = Date{d29,mi,y2024};
	auto datey600 = Date{dy5,m2,yi};
	auto datey25091986 = Date("25.09.1986");
	assert(!dated45.Valid());
	assert(!datem16.Valid());
	assert(!datey600.Valid());
	assert(datey25091986.Valid());
	assert(!Date("").Valid());
	auto dateytest = Date("TEST");
	assert(!dateytest.Valid());

	auto today = Date::Today();
	std::cout<<"TODAY"<<today<<std::endl;
	//auto tq = ToQuantity<Year>(std::cin, (Year)today);

    std::istringstream is("12 12 1999 13 1 1 2022");

	auto tq1 = ToQuantity<Month>(is,std::cout);
	auto tq2 = ToQuantity<Day>(is,std::cout);
	auto tq3 = ToQuantity<Year>(is,std::cout);
	assert(tq1.Value()==12);
	assert(tq2.Value()==12);
	assert(tq3.Value()==1999);
    
    std::istringstream isp("12.12.1999");
	auto cp = Date::Create(isp);
	
	assert(12==cp.M());
	assert(1999==cp.Y());
	
//    std::istringstream ise("11 11 1998");
//	auto ce = Date::Create(ise);
//	
//	assert(11==ce.M());
//	assert(1998==ce.Y());
//	std::cout<<"\nISP"""<<std::endl;
//
//	std::cout<<"TQ m: "<<tq1<<" d "<<tq2<<" "<<tq3<<std::endl;
//	Day::Create(is,std::cout);
//	auto c = Date::Create(is,std::cout);
//	std::cout<<"\n"<<c<<std::endl;
	std::cout<<"END"<<std::endl;

	return 0;
}
