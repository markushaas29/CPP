#include <memory>
#include <fstream>
#include "AncilliaryRentalCostItems.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.h"
#include "../../Quantity/Quantity.h"
#include "../../Calculator/Calculator.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../File/Raiba.hpp"
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
	AncilliaryRentalCostsContainer(): year(2021)
	{ 
		Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<std::endl;
		auto iban = GetStage<S,IBAN>();
		auto stageTransfers = Bank::Get<Bank::Raiba<0>>(iban);
		auto transfers = stageTransfers.GetTransferOf(year);
		
		auto advance = GetStage<S,Advance>().GetQuantity();
		auto heatingCosts = GetStage<S,IncidentalHeatingCosts>().GetQuantity();
		auto advancePays = (advance + heatingCosts) * paymentsPerYear;

		
		auto fs = std::make_unique<std::ofstream>(std::string(StageT::StageName)+ ".txt");
		for(auto t : *transfers)
			*fs<<*t<<std::endl;
		*fs<<"\n--------Output-----\n"<<std::endl;
		fs = this->Calculate(std::move(fs),*items);
		
		*fs<<"\n--------Advance-----\n"<<std::endl;
		*fs<<"\nHeating:\t"<<advance<<std::endl;
		*fs<<"\nAdvance:\t"<<heatingCosts<<std::endl;
		*fs<<"\nTotal:\t"<<advancePays<<std::endl;
				
		*fs<<"\n--------Total Sum-----\n"<<std::endl;
		*fs<<"\nSum: "<<total<<std::endl;
		
		*fs<<"\n--------Result-----\n"<<std::endl;
		*fs<<"\nSum: "<<Subtraction::Calculate(advancePays,total)<<std::endl;
		fs->close();
	}	
	
	~AncilliaryRentalCostsContainer()	{  }
	template <size_t I = 0, typename... Ts>
	constexpr std::unique_ptr<std::ofstream> Calculate(std::unique_ptr<std::ofstream> fs, std::tuple<Ts...> tup) 
	{
		if constexpr(I == sizeof...(Ts))    
			return fs;
		else 
		{
			auto item = std::get<I>(*items);
			auto y = DateTimes::Year(2021);
			auto result = decltype(item)::Calculate(y);
			
			total = total + result.Get();
			Logger::Log<Info>("Sum: ", result.Get()," Total", total);
			(*fs)<<decltype(item)::Result(y)<<std::endl;
			
			return Calculate<I + 1>(std::move(fs), *items);
		}
	}
	
	std::ostream& Display(std::ostream& os) {	return os;	}
	YearType year;
	const Quantity<Scalar> paymentsPerYear = Quantity<Scalar>{12};
	Quantity<Sum> total = Quantity<Sum>(0.0);
};

//~ template<typename C, typename S = T::char_<'\t'>>
//~ std::ostream& operator<<(std::ostream& strm, const AncilliaryRentalCostsContainer<C,Y>& c){	return c.Display(strm); }


#endif
