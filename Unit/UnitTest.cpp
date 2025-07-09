#include <cassert>
#include <iostream>
#include "Unit.hpp"
#include "UnitRatio.hpp"
#include "../CSV/Element.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"
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
	std::cout<<"Ratio Factor: "<<decltype(ur12)::Hours::Factor<<std::endl;
	std::cout<<"Factor 1: "<<decltype(ur12)::Factor<<std::endl;
	assert(decltype(ur12)::Factor<0.016667);
	assert(decltype(ur12)::Min::N==3);
	assert(decltype(ur12)::Hours::N==-2);
	assert(decltype(ur12)::Min::Num==216000);
	assert(decltype(ur12)::Hours::Num==1);
	assert(decltype(ur12)::Hours::Denom==12960000);
	assert(decltype(ur12)::Hours::Factor==(1.0/12960000));
	
	std::cout<<"Ratio Sign: "<<TransformRatio<UnitRatio<1,-2>, UnitRatio<2,-1>, MultiplyPolicy>::ResultingUnit::SiUnit()<<std::endl;
	assert(decltype(ur12)::Unit()=="min³h^-2");
 	
	auto urL = TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::Type();
	assert((PowUnit<std::abs(std::abs(1)-1),decltype(urL)>::Type().Unit())=="h²\"l");
//	assert((TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::ResultingUnit::SiUnit())=="m^-4s^-8");
	assert(decltype(urL)::Factor==329.184);
	assert(decltype(urL)::Hours::N==2);
	assert(decltype(urL)::Hours::Num==12960000);
	assert(decltype(urL)::Hours::Denom==1);
	assert(decltype(urL)::Liter::Num==1);
	assert(decltype(urL)::Liter::Denom==1000);
	assert(decltype(urL)::Inch::Num==254);
	assert(decltype(urL)::Inch::Denom==10000);
	
	auto lpH = TransformRatio<UnitRatio<0,-1>, UnitRatio<0,0,0,0,0,1,0>, MultiplyPolicy>::Type();
	std::cout<<"Liter / h: "<<(PowUnit<std::abs(std::abs(1)-1),decltype(lpH)>::Type().Unit())<<std::endl;
	assert((PowUnit<std::abs(std::abs(1)-1),decltype(lpH)>::Type().Unit())=="h^-1l");
	
	auto lpMin = TransformRatio<UnitRatio<-1>, UnitRatio<0,0,0,0,0,1,0>, MultiplyPolicy>::Type();
	std::cout<<"Liter / h: "<<(PowUnit<std::abs(std::abs(1)-1),decltype(lpH)>::Type().Unit())<<std::endl;
	assert((PowUnit<std::abs(std::abs(1)-1),decltype(lpMin)>::Type().Unit())=="min^-1l");
	
//	auto urL2 = TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,-2>, MultiplyPolicy>::Type();
//	std::cout<<"Ratio: "<<TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,-2>, MultiplyPolicy>::ResultingUnit::SiUnit()<<std::endl;
//	assert((TransformRatio<UnitRatio<0,2,0,1>, UnitRatio<0,0,0,0,0,-2>, MultiplyPolicy>::R::SiUnit())=="m^5s^-4");
	
	auto mps = Transform<Unit<0,1>, Unit<0,0,0,-1>, MultiplyPolicy>::Type();
	assert(decltype(mps)::SiUnit()=="ms^-1");
	std::cout<<"m/s: "<<decltype(mps)::SiUnit()<<std::endl;
	auto mph = TransformRatio<UnitRatio<0,-1>, UnitRatio<0,0,0,0,1>, MultiplyPolicy>::Type();
	assert(decltype(mph)::Unit()=="h^-1mi");
	using ru = TransformRatio<UnitRatio<0,-1>, UnitRatio<0,0,0,0,1>, MultiplyPolicy>::ResultingUnit;
	assert(ru::SiUnit()=="m^-1s");
	using scalar = Transform<ru, decltype(mps), MultiplyPolicy>::Type;
	std::cout<<"mi/h: "<<scalar::SiUnit()<<std::endl;
	assert(scalar::SiUnit()=="");
	
	auto m3ps = Transform<Unit<0,3>, Unit<0,0,0,-1>, MultiplyPolicy>::Type();
	assert(decltype(m3ps)::SiUnit()=="m^3s^-1");
	auto lpmin = TransformRatio<UnitRatio<-1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::Type();
	assert(decltype(lpmin)::Unit()=="min^-1l");
	using ru2 = TransformRatio<UnitRatio<-1>, UnitRatio<0,0,0,0,0,1>, MultiplyPolicy>::ResultingUnit;
	std::cout<<"m/s: "<<ru2::SiUnit()<<std::endl;
	assert(ru2::SiUnit()=="m^-3s");
	using scalar = Transform<ru2, decltype(m3ps), MultiplyPolicy>::Type;
	std::cout<<"mi/h: "<<scalar::SiUnit()<<std::endl;
	assert(scalar::SiUnit()=="");
	
	Quantity<Length,Kilo>(1.001);
	auto h24 = Quantity<Time,UnitRatio<0,1>,double>(24);
	std::cout<<"24h: "<<UnitRatio<0,1>::Sign<<std::endl;
	assert((UnitRatio<0,1>::Sign=="h"));
	std::cout<<"24h: "<<h24<<std::endl;
	std::cout<<"24h: "<<h24.Data()<<std::endl;
	
	std::cout<<"END"<<std::endl;
    return 0;
}
};
