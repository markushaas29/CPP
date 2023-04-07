#include <memory>
#include <fstream>
#include "AncilliaryRentalCostItems.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Common/Configuration.hpp"
#include "../../File/Raiffeisenbank.hpp"
#include "../../File/Account.hpp"

#ifndef ANCILLIARYRENTALCOSTSCONTAINER_HPP
#define ANCILLIARYRENTALCOSTSCONTAINER_HPP
using YearType = DateTimes::Year;

template<typename S>
class AncilliaryRentalCostsContainer
{
public:
	using Type = AncilliaryRentalCostsContainer;
	using StageT = S;
	using Items = CalculationItems<S>;
	template<typename T>
	using ValueType = std::shared_ptr<T>;
	
	static AncilliaryRentalCostsContainer& Instance()
	{
		static AncilliaryRentalCostsContainer instance;
		return instance;
	}
		
private:
	std::unique_ptr<Items> items = std::unique_ptr<Items>();
	AncilliaryRentalCostsContainer(): year(2022)
	{ 
		auto fs = std::make_unique<std::ofstream>(std::string(StageT::StageName)+ ".txt");
		fs = this->printHead(std::move(fs));
		
		fs = this->Calculate(std::move(fs));
		
		fs = this->printResult(std::move(fs));
		
		fs->close();
	}	
	
	decltype(auto) printResult(std::unique_ptr<std::ofstream> fs)
	{
		auto advance = GetStage<S,Advance>().GetQuantity();
		auto heatingCosts = GetStage<S,IncidentalHeatingCosts>().GetQuantity();
		auto advancePays = (advance + heatingCosts) * paymentsPerYear;
		
		*fs<<"\n--------Advance-----\n"<<std::endl;
		*fs<<"\n"<<Advance::Key<<":\t"<<advance<<std::endl;
		*fs<<"\n"<<IncidentalHeatingCosts::Key<<":\t"<<heatingCosts<<std::endl;
		*fs<<"\nTotal:\t"<<advancePays<<std::endl;
		
				
		*fs<<"\n--------Total Sum-----\n"<<std::endl;
		*fs<<"\nSum: "<<total<<std::endl;
		
		*fs<<"\n--------TotalResult-----\n"<<std::endl;
		*fs<<"\nSum: "<<Addition::Calculate(advancePays,total)<<std::endl;
		
		*fs<<"\n--------TotalItemCosts-----\n"<<std::endl;
		*fs<<"\nSum: "<<totalOut<<std::endl;
		*fs<<"\nSum: "<<Division::Calculate(total,totalOut)<<std::endl;
		
		return fs;
	}
	
	decltype(auto) printHead(std::unique_ptr<std::ofstream> fs)
	{
		fs = S::Instance().PrintQuantities(std::move(fs));
		auto iban = GetStage<S,IBAN>();
		auto stageTransfers = Bank::Get<Configuration::AncilliaryRentalCosts::AccountType>(iban);
		auto transfers = stageTransfers->GetTransferOf(year);
		
		for(auto t : *transfers)
			*fs<<*t<<std::endl;
		
		return fs;
	}
	
	~AncilliaryRentalCostsContainer()	{  }
	
	template <size_t I = 0>
	constexpr std::unique_ptr<std::ofstream> Calculate(std::unique_ptr<std::ofstream> fs) 
	{
		if constexpr(I ==std::tuple_size_v<Items>)    
			return fs;
		else 
		{
			auto item = std::get<I>(*items);
			using ItemType = decltype(item);
			auto result = ItemType::Calculate(year);
			
			total = total + result;
			auto itemResult = ItemType::Result(year);
			totalOut = totalOut + ItemType::Result(year).ItemCosts();
			(*fs)<<ItemType::Result(year)<<std::endl;
			
			return Calculate<I + 1>(std::move(fs));
		}
	}

	std::ostream& Display(std::ostream& os) {	return os;	}
	YearType year;
	const Quantity<Scalar> paymentsPerYear = Quantity<Scalar>{12};
	Quantity<Sum> total = Quantity<Sum>(0.0);
	Quantity<Sum> totalOut = Quantity<Sum>(0.0);
};

#endif
