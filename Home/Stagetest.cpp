#include <iostream>
#include "Counter.hpp"
#include "CounterContainer.hpp"
#include "Stage.hpp"
#include "StageContainer.hpp"
#include "House.hpp"
#include "../File/AccountQuery.hpp"
#include "AncilliaryRentalCosts/AncilliaryRentalCostsContainer.hpp"
#include "../Tuple/Tuple.hpp"
#include "../Unit/Unit.hpp"
#include "../File/Repository.hpp"
#include "../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	House<HouseConfiguration>::Instance();
    std::cout<<"t: "<<Top::StageName<<std::endl;
    std::cout<<"t: "<<Top::Number<<std::endl;
    
    std::cout<<"t: "<<ApartmentArea::Key<<std::endl;
	
	ApartmentArea aa(7);
	Rooms r(6);
	
	StageContainerType::Instance().Display(std::cout);
    std::cout<<"m: "<<Middle::Instance().StageName<<std::endl;
    
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
	
	//~ CSV::Get<CEHouse>();
	
	StageContainerType::Instance().Calculate<AncilliaryRentalCostsContainer>();
    std::cout<<"END"<<std::endl;

	std::ofstream of{"All.txt"};
	CSV::Repository<CSV::CommonRepositoryConfiguration>::Instance().Display(of);
	
	constexpr static DateTimes::Year Y{2022};    
	using AccountType = Configuration::AncilliaryRentalCosts::AccountType;         
    auto rc = Bank::GetTransfersOf<AccountType>(Y);
    
    constexpr Bank::ItemQuery<AccountType,DateTimes::Year, DateTimes::Month> iq{Y,DateTimes::July};
    
    auto riq = iq.Execute();
    
    for(auto cx : *rc)
		std::cout<<*cx<<std::endl;
	
	auto items = riq.Items();
	 for(auto cx : *items)
		std::cout<<*cx<<std::endl;
	
	std::cout<<riq.GetSum()<<std::endl;
    return 0;
}
