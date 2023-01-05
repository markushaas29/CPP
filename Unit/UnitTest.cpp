#include <cassert>
#include <iostream>
#include "Unit.hpp"
#include "SIPrefix.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    std::cout<<Mass::Mass::Name<<" "<<Mass::Mass::Sign<<" "<<Mass::Mass::N<<" "<<std::endl;
    std::cout<<Sum::Sum::Name<<" "<<Sum::Sum::Sign<<" "<<std::endl;
    std::cout<<Length::Length::Name<<" "<<Length::Length::Sign<<" "<<std::endl;
	
	auto u1 = Mass();
	auto u2 = Unit<0,0,2>();
// 	auto u3 = Transform<u1::Type,u2::Type, MultiplyPolicy>();
	using u3 = Transform<Sum,Unit<2>, MultiplyPolicy>::Type;

	std::cout<<u3::Sum::N<<std::endl;
// 	std::cout<<UnitSign<int, 0, 0,0, 0,0, 0,0, 0>::Get()<<std::endl;
	std::cout<<UnitSign<Mass>::Get()<<std::endl;
	std::cout<<UnitSign<decltype(u2)>::Get()<<std::endl;
    
	std::cout<<"IMPLIZIT"<<std::endl;
	
	std::cout<<Mass::Sign<<std::endl;
	std::cout<<decltype(u2)::Sign()<<std::endl;
	std::cout<<decltype(u2)::SiUnit()<<std::endl;
	
	std::cout<<"Work"<<std::endl;
	std::cout<<Work::Sign()<<std::endl;
	std::cout<<Work::SiUnit()<<std::endl;
	
	std::cout<<"Volume"<<std::endl;
	std::cout<<Volume::Sign()<<std::endl;
	std::cout<<Volume::SiUnit()<<std::endl;
	
	std::cout<<"Kilo "<<Milli::Name<<std::endl;
    std::cout<<"Milli "<<Milli::Sign<<std::endl;
    std::cout<<"Milli "<<Milli::Factor<<std::endl;

	std::cout<<"H "<<Hours::Name<<std::endl;
    std::cout<<"H "<<Hours::Sign<<std::endl;
    std::cout<<"H "<<Hours::Factor<<std::endl;
	
	std::cout<<"D "<<Days::Name<<std::endl;
    std::cout<<"D "<<Days::Sign<<std::endl;
    std::cout<<"D "<<Days::Factor<<std::endl;
	
	std::cout<<"Kilo "<<Kilo::Name<<std::endl;
    std::cout<<"Kilo "<<Kilo::Sign<<std::endl;
    std::cout<<"Kilo "<<Kilo::Factor<<std::endl;
	
	std::cout<<"Hekto "<<Hekto::Name<<std::endl;
    std::cout<<"Hekto "<<Hekto::Sign<<std::endl;
	
	std::cout<<"Deka "<<Deka::Name<<std::endl;
    std::cout<<"Deka "<<Deka::Sign<<std::endl;
	
	std::cout<<"decltype(u2)::Time"<<decltype(u2)::Time::N<<std::endl;
	auto f1 = CalculateFactor<decltype(u2)::Time,decltype(u2)::Time>();
	assert(decltype(u2)::Time::N==0);
	assert(f1==1);
	f1 = CalculateFactor<decltype(u2)::Mass,decltype(u2)::Time>();
	assert(decltype(u2)::Mass::N==2);
	assert(decltype(u2)::Time::N==0);
	assert(f1==0);
	f1 = CalculateFactor<decltype(u1)::Mass,decltype(u2)::Mass>();
	assert(decltype(u1)::Mass::N==1);
	assert(decltype(u2)::Mass::N==2);
	assert(f1==0.5);
    
	auto U1 = Unit<0,0,1>();
	auto U2 = Unit<0,0,2>();
    auto b = IsSameBaseUnit<decltype(U1), decltype(U2)>();
	assert(b);
    
	auto U3 = Unit<0,0,3>();
    b = IsSameBaseUnit<decltype(U3), decltype(U2)>();
	assert(b);

	auto U4 = Unit<0,1>();
    b = IsSameBaseUnit<decltype(U4), decltype(U2)>();
	assert(!b);
	
	auto U5 = Unit<0,0,2,1>();
    b = IsSameBaseUnit<decltype(U5), decltype(U2)>();
	assert(b);
    
	std::cout<<"END"<<std::endl;
    return 0;
}
