#include <iostream>
#include "Counter.hpp"
#include "CounterContainer.hpp"
#include "Stage.hpp"
#include "StageContainer.hpp"
#include "House.hpp"
#include "AncilliaryRentalCosts/AncilliaryRentalCostsContainer.hpp"
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
    std::cout<<"t: "<<Top::Name<<std::endl;
    std::cout<<"t: "<<Top::Number<<std::endl;
    
    std::cout<<"t: "<<ApartmentArea::Key<<std::endl;
	
	ApartmentArea aa(7);
	Rooms r(6);
	
	StageContainerType::Instance().Display(std::cout);
    std::cout<<"m: "<<Middle::Instance().Name<<std::endl;
    
    std::cout<<"AREA"<<std::endl;
	std::cout<<"t: "<<GetStage<Top,ApartmentArea>()<<std::endl;
    std::cout<<"m: "<<GetStage<Middle,ApartmentArea>().GetQuantity()<<std::endl;
    std::cout<<"b: "<<GetStage<Bottom,ApartmentArea>().GetQuantity()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<ApartmentArea>()<<std::endl;
	
    std::cout<<"\nPersons"<<std::endl;
	std::cout<<"t: "<<GetStage<Top,Persons>().GetQuantity()<<std::endl;
    std::cout<<"m: "<<GetStage<Middle,Persons>().GetQuantity()<<std::endl;
    std::cout<<"b: "<<GetStage<Bottom,Persons>().GetQuantity()<<std::endl;
    std::cout<<"TOTAL: "<<StageContainerType::Instance().GetTotal<Persons>()<<std::endl;
	
	StageContainerType::Instance().Calculate<AncilliaryRentalCostsContainer>();
    std::cout<<"END"<<std::endl;

    return 0;
}
