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
	
    
	std::cout<<"END"<<std::endl;
    return 0;
}
