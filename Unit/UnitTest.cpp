#include <cassert>
#include <iostream>
#include "Unit.hpp"
#include "UnitRatio.hpp"
//#include "SIPrefix.hpp"
struct UnitTest
{
int Run()
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
	
	std::cout<<"Energy"<<std::endl;
	std::cout<<Energy::Sign()<<std::endl;
	std::cout<<Energy::SiUnit()<<std::endl;
	
	std::cout<<"Volume"<<std::endl;
	std::cout<<Volume::Sign()<<std::endl;
	std::cout<<Volume::SiUnit()<<std::endl;
	
	std::cout<<"decltype(u2)::Time"<<decltype(u2)::Time::N<<std::endl;
	auto f1 = CalculateFactor<decltype(u2)::Time,decltype(u2)::Time>();
	assert(decltype(u2)::Time::N==0);
	assert(f1==-1);
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
	
	auto n1 = Unit<1,2,-2>();
	auto n2 = Unit<2,4,-4>();
    auto nb = IsSameBaseUnit<decltype(n1), decltype(n2)>();
	assert(nb);
    
	auto ur1 = UnitRatio<1,-2>();
	auto ur2 = UnitRatio<2,-1>();
 	auto ur12 = TransformRatio<UnitRatio<1,-1>, UnitRatio<2,-1>, MultiplyPolicy>::Type();
	std::cout<<"Ratio: "<<decltype(ur12)::Hours::BaseNum<<"Ratio: "<<decltype(ur12)::Hours::BaseDenom<<std::endl;
	std::cout<<"Ratio Factor: "<<decltype(ur12)::Hours::Denom<<"\t"<<Math::Pow<3600,3>::Result<<std::endl;
	std::cout<<"Ratio Factor: "<<(3600 > 1 ? (-2 > 0 ? Math::Pow<3600,-2>::Result : Math::Pow<2,-(-2)>::Result) : 1)<<std::endl;
	assert(decltype(ur12)::Min::N==3);
	assert(decltype(ur12)::Hours::N==-2);
	assert(decltype(ur12)::Min::Num==216000);
	assert(decltype(ur12)::Hours::Num==1);
	assert(decltype(ur12)::Hours::Denom==12960000);
	
	std::cout<<"Ratio Sign: "<<TransformRatio<UnitRatio<1,-2>, UnitRatio<2,-1>, MultiplyPolicy>::R::SiUnit()<<std::endl;
	assert(decltype(ur12)::URatio()=="min³h^-2");
 	
	auto urL = TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::Type();
	std::cout<<"Ratio: "<<TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::R::SiUnit()<<std::endl;
	assert((TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::R::SiUnit())=="m^-4s^-4");
	std::cout<<"Ratio: "<<decltype(urL)::Hours::Num<<std::endl;
	assert(decltype(urL)::Hours::N==2);
	assert(decltype(urL)::Hours::Num==12960000);
	
	std::cout<<"END"<<std::endl;
    return 0;
}
};
