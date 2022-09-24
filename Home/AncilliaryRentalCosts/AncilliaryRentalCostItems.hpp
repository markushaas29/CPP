#include <tuple>
#include <memory>
#include "AncilliaryRentalCostResult.hpp"
#include "../Stage.hpp"
#include "../../Calculator/Calculator.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.h"
#include "../../Quantity/Quantity.h"
#include "../../CSV/CSV.hpp"
#include "../../File/Info.hpp"
#include "../../File/Node.hpp"
#include "../../Unit/SIPrefix.hpp"
#include "../../Wrapper/Wrapper.hpp"
#include "../../Home/StageQuantities.hpp"
#include "../../File/Raiba.hpp"
#include "../../File/Account.hpp"

#ifndef ANCILLIARYRENTALCOST_HPP
#define ANCILLIARYRENTALCOST_HPP

template<typename Derived, typename Q>
struct CalculatorConfiguration
{
	inline static const Key AccountKey = Key("");
	using Type = Derived;
	using QuantityType = Q;
	constexpr static const char* Name = "";//Derived::Name; 
};

template<typename StageT,typename Derived, typename Q>
struct AncilliaryRentalCostItemBase
{
	using Type = Derived;
	using StageType = StageT;
	using StageQuantity = Q;
	using AccountType = Bank::Raiba<0>;
	using ResultType =  AncilliaryRentalCostItemResult<Derived,StageType,StageQuantity,AccountType>;
	using MapType = std::map<DateTimes::Year,ResultType>;
	constexpr static const char* Name = "";
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<AccountType>(Derived::iban);
		auto transfers = account[year];
		
		auto sum = TotalSum(transfers->cbegin(), transfers->cend());
		
		auto denom = StageContainerType::Instance().GetTotal<Q>();
		auto num = GetStage<StageType,Q>().GetQuantity();
		results->insert({year,ResultType{std::move(transfers),num,denom,sum,year}});
		
		return (*results)[year].Get();
	}
	
	static const ResultType& Result(const DateTimes::Year& y){ return (*results)[y]; }
	static std::ostream& Display(std::ostream& os)
	{
		os<<results->cbegin()->first<<" Result "<<std::endl;
		return os;
	}
	
protected:
	template <typename Acc, typename T, typename... Categories>
	static decltype(auto) InsertSpecifiedTransfers(const Acc& account, std::unique_ptr<T>& transfers, Categories... categories)
	{ 
		auto specifiedtransfers = account.GetTransferOf(categories...);
		
		if(specifiedtransfers->begin() != specifiedtransfers->end())
			transfers->insert(transfers->end(), specifiedtransfers->begin(), specifiedtransfers->end());
		else
			Logger::Log<Info>("No Transfers found for categories: ", categories...);
	}
		
	inline static std::unique_ptr<MapType> results = std::make_unique<MapType>();	
	
	template<typename It>
	static decltype(auto) TotalSum(It begin, It end)
	{
		auto acc = Bank::GetTransfer<Quantity<Sum>>(**(begin));
		std::for_each(begin+1, end, [&](const auto t){ acc = acc + Bank::GetTransfer<Quantity<Sum>>(*t); });
		return acc;
	}
};

template<typename S,typename D, typename Q>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemBase<S,D,Q>& s){	return s.Display(out);	}

template<typename S>
struct BuildingInsurance: AncilliaryRentalCostItemBase<S, BuildingInsurance<S>, IndividualUnit> 
{ 
	constexpr static const char* Name = "BuildingInsurance"; 
	constexpr static const char* Identifier = "SV Gebaeudeversicherung"; 
	inline static const IBAN iban{"DE97500500000003200029"};	
};

template<typename S>
struct WasteFees: AncilliaryRentalCostItemBase<S,WasteFees<S>, Persons> 
{ 
	constexpr static const char* Name = "WasteFees"; 
	constexpr static const char* Identifier = "Abfallwirtschaftsbetrieb"; 
	inline static const IBAN iban{"DE44600501010008017284"};	
};

template<typename S>
struct BuildingCleaning: AncilliaryRentalCostItemBase<S,BuildingCleaning<S>, BuildingCleaningProportion> 
{ 
	constexpr static const char* Name = "BuildingCleaning"; 
	constexpr static const char* Identifier = "Alles Proper"; 
	inline static const IBAN iban{"DE44600501010008017284"};	
};

template<typename S>
struct Heating: AncilliaryRentalCostItemBase<S,Heating<S>, HeatingProportion> 
{ 
	using Base =  AncilliaryRentalCostItemBase<S,Heating<S>, HeatingProportion>;
	constexpr static const char* Name = "Heating"; 
	constexpr static const char* Identifier = "Erdgas Suedwest GmbH / EnBW Energie Bad-Wuertt AG"; 
	constexpr static const char* IdentifierEnergy = "EnBW Energie Bad-Wuertt AG"; 
	constexpr static const char* IdentifierGas = "Erdgas Suedwest GmbH"; 
	constexpr static const char* Invoice = "Rechnung"; 
	constexpr static const char* AdvancePayment = "Abschlag"; 
	inline static const IBAN ibanGas{"DE68600501010002057075"};	
	inline static const IBAN ibanEnergy{"DE56600501017402051588"};	
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto invoiceEntry = Entry(Invoice);
		auto paymentEntry = Entry(AdvancePayment);
		
		auto accGas = Bank::Get<typename Base::AccountType>(ibanGas);
		auto transfers = accGas.GetTransferOf(paymentEntry,year);
		Base::InsertSpecifiedTransfers(accGas, transfers, invoiceEntry,year.Next());
		
		auto accEnergy = Bank::Get<typename Base::AccountType>(ibanEnergy);
		Base::InsertSpecifiedTransfers(accEnergy, transfers, paymentEntry,year.Next());
		Base::InsertSpecifiedTransfers(accEnergy, transfers, invoiceEntry,year.Next());
		
		auto sum = Base::TotalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<HeatingProportion>();
		auto num = GetStage<S,HeatingProportion>().GetQuantity();
		Base::results->insert({year,typename Base::ResultType{std::move(transfers),num,denom,sum,year}});
		
		return (*Base::results)[year].Get();
	}
};


template<typename S>
struct ChimneySweeper: AncilliaryRentalCostItemBase<S,ChimneySweeper<S>, IndividualUnit> 
{ 
	constexpr static const char* Name = "ChimneySweeper"; 
	constexpr static const char* Identifier = "Sascha Schneider"; 
	inline static const IBAN iban{"DE82660501011021592702"};	
};

struct LocalCommunity
{
	constexpr static const char* Identifier = "Gemeindekasse Dettenheim";	
	inline static const IBAN iban{"DE12660623660000005703"};	
};

template<typename S, typename Server = LocalCommunity>
struct PropertyTax: public AncilliaryRentalCostItemBase<S, PropertyTax<S,Server>, ApartmentArea> 
{ 
	constexpr static const char* Identifier = Server::Identifier;	
	inline static const IBAN iban = Server::iban;
	constexpr static const char* Name = "PropertyTax"; 
	constexpr static const char* Cause = "Grundsteuer"; 
};

template<typename S, typename Server = LocalCommunity>
struct Sewage: public AncilliaryRentalCostItemBase<S, Sewage<S,Server>, WaterCounter>
{ 
	using Base = AncilliaryRentalCostItemBase<S, Sewage<S,Server>, WaterCounter>; 
	constexpr static const char* Identifier = Server::Identifier;	
	inline static const IBAN iban = Server::iban;
	constexpr static const char* Cause = "Abschlag/Abwasser"; 
	constexpr static const char* Name = "Sewage"; 
	constexpr static const char* Invoice = "Rechnung/Abwasser"; 
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<typename Base::AccountType>(iban);
		auto transfers = account.GetTransferOf(Entry(Cause),year);		
		Base::InsertSpecifiedTransfers(account, transfers, Entry(Invoice),year.Next());

		auto sum = Base::TotalSum(transfers->cbegin(), transfers->cend());
		auto stageColdWater = S::ColdWaterCounter::Instance().Get(Difference());
		auto stageHotWater = S::HotWaterCounter::Instance().Get(Difference());
				
		auto houseHotWater = StageContainerType::Instance().GetCounterTotal<Hot,Difference>();
		auto houseColdWater = StageContainerType::Instance().GetCounterTotal<Cold,Difference>();
		
		auto stageWater = stageColdWater + stageHotWater;
		auto houseWater = houseHotWater + houseColdWater;

		Logger::Log()<<"Div"<<QuantityRatio::Calculate(stageColdWater.Get(),houseWater.Get(),sum)<<std::endl;
		Base::results->insert({year,typename Base::ResultType{std::move(transfers),stageWater.Get(),houseWater.Get(),sum,year}});
		
		return (*Base::results)[year].Get();
	}
};

template<typename S>
using CalculationItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>, BuildingCleaning<S>, Heating<S>>;

#endif
