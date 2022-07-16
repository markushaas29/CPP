#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Counter.hpp"
#include "CounterContainer.hpp"
#include "Stage.hpp"
#include "House.hpp"
#include "StageContainer.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Calculator/Calculator.hpp"
#include "../Tuple/Tuple.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"
#include "../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	House<HouseConfiguration>::Instance();
// 	Top::Instance();
    std::cout<<"t: "<<Top::Name<<std::endl;
    std::cout<<"t: "<<Top::Number<<std::endl;
    
    std::cout<<"t: "<<ApartmentArea::Key<<std::endl;
	
	ApartmentArea aa(7);
	Rooms r(6);
	
	StageContainerType::Instance().Display(std::cout);
// 	auto m = StageContainerType::Instance().Get<Middle>();
    std::cout<<"m: "<<Middle::Instance().Name<<std::endl;
    
    std::cout<<"AREA"<<std::endl;
	std::cout<<"t: "<<GetStage<ApartmentArea,Top::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"m: "<<GetStage<ApartmentArea,Middle::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"b: "<<GetStage<ApartmentArea,Bottom::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<ApartmentArea>()<<std::endl;
	
    std::cout<<"\nPersons"<<std::endl;
	std::cout<<"t: "<<GetStage<Persons,Top::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"m: "<<GetStage<Persons,Middle::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"b: "<<GetStage<Persons,Bottom::Configuration>().GetQuantity()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<Persons>()<<std::endl;
	
	StageContainerType::Instance().Calculate();
    std::cout<<"END"<<std::endl;

    return 0;
}
