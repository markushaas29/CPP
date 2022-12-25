#include <tuple>
#include <memory>
#include "AncilliaryRentalCostResult.hpp"
#include "../Stage.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../File/Info.hpp"
#include "../../File/Node.hpp"
#include "../../Unit/SIPrefix.hpp"
#include "../../Wrapper/Wrapper.hpp"
#include "../../Home/StageQuantities.hpp"
#include "../../File/Raiffeisenbank.hpp"
#include "../../File/Account.hpp"
#include "../../Common/Configuration.hpp"

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
	using AccountType = Configuration::AncilliaryRentalCosts::AccountType;
	using ResultType =  AncilliaryRentalCostItemResult<Derived,StageType,StageQuantity,AccountType>;
	using MapType = std::map<DateTimes::Year,ResultType>;
	constexpr static Name TypeIdentifier = Name{""};
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<AccountType>(Derived::iban);
		typename ResultType::TransfersPtr transfers;
		typename ResultType::SumType sum;

		if(account)
		{
			transfers = (*account)[year];
			sum = TotalSum(transfers->cbegin(), transfers->cend());
		}
		else
		{
			transfers = std::make_unique<typename ResultType::Transfers>();
			Logger::Log<Error>("No transfers for ", Derived::Identifier, " sum is ", sum);
		}
		
		auto denom = StageContainerType::Instance().GetTotal<Q>();
		auto num = GetStage<StageType,Q>().GetQuantity();
		results->insert({year,ResultType{std::move(transfers),num,denom,sum,year}});
		
		return (*results)[year].Get();
	}
	
	static const ResultType& Result(const DateTimes::Year& y){ return (*results)[y]; }
	static std::ostream& Display(std::ostream& os){	return os<<results->cbegin()->first<<" Result "<<std::endl;	}
	
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
		auto acc = Quantity<Sum>{0};
		if(begin == end)
		{
			Logger::Log<Error>("No transfers for ", Derived::TypeIdentifier,"and sum is ", acc);
			return acc;
		}
		
		acc = Bank::GetTransfer<Quantity<Sum>>(**(begin));
		std::for_each(begin+1, end, [&](const auto t){ acc = acc + Bank::GetTransfer<Quantity<Sum>>(*t); });
				
		return acc;
	}
};

template<typename S,typename D, typename Q>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemBase<S,D,Q>& s){	return s.Display(out);	}

template<typename S>
struct BuildingInsurance: AncilliaryRentalCostItemBase<S, BuildingInsurance<S>, IndividualUnit> 
{ 
	using Base = AncilliaryRentalCostItemBase<S, BuildingInsurance<S>,IndividualUnit>; 
	constexpr static Name TypeIdentifier{"BuildingInsurance"}; 
	constexpr static const char* Identifier = "SV Gebaeudeversicherung"; 
	inline static constexpr IBAN iban{"DE97500500000003200029"};	
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<typename Base::AccountType>(iban);
		auto transfers = account->GetTransferOf(year);		

		auto sum = Base::TotalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<IndividualUnit>() + Quantity<Scalar>(1);
		auto num = GetStage<S,IndividualUnit>().GetQuantity();
		
		if constexpr (std::is_same<S,Top>::value)
			num = num + Quantity<Scalar>(1);
				
		Base::results->insert({year,typename Base::ResultType{std::move(transfers),num,denom,sum,year}});
		
		return (*Base::results)[year].Get();
	}
};

template<typename S>
struct WasteFees: AncilliaryRentalCostItemBase<S,WasteFees<S>, Persons> 
{ 
	constexpr static Name TypeIdentifier{"WasteFees"}; 
	constexpr static const char* Identifier = "Abfallwirtschaftsbetrieb"; 
	inline static constexpr IBAN iban{"DE44600501010008017284"};	
};

template<typename S>
struct BuildingCleaning: AncilliaryRentalCostItemBase<S,BuildingCleaning<S>, BuildingCleaningProportion> 
{ 
	constexpr static Name TypeIdentifier{"BuildingCleaning"}; 
	constexpr static const char* Identifier = "Alles Proper"; 
	inline static constexpr IBAN iban{"DE05100110012620778704"};	
};

template<typename S>
struct Heating: AncilliaryRentalCostItemBase<S,Heating<S>, HeatingProportion> 
{ 
	using Base =  AncilliaryRentalCostItemBase<S,Heating<S>, HeatingProportion>;
	constexpr static Name TypeIdentifier = Name{"Heating"}; 
	constexpr static const char* Identifier = "Erdgas Suedwest GmbH / EnBW Energie Bad-Wuertt AG"; 
	constexpr static const char* IdentifierEnergy = "EnBW Energie Bad-Wuertt AG"; 
	constexpr static const char* IdentifierGas = "Erdgas Suedwest GmbH"; 
	constexpr static Entry Invoice{"Rechnung"}; 
	constexpr static Entry AdvancePayment{"Abschlag"}; 
	inline static constexpr IBAN ibanGas{"DE68600501010002057075"};	
	inline static constexpr IBAN ibanEnergy{"DE56600501017402051588"};	
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto accGas = Bank::Get<typename Base::AccountType>(ibanGas);
		auto transfers = accGas->GetTransferOf(AdvancePayment,year);
		Base::InsertSpecifiedTransfers(*accGas, transfers, Invoice,year.Next());
		
		auto accEnergy = Bank::Get<typename Base::AccountType>(ibanEnergy);
		Base::InsertSpecifiedTransfers(*accEnergy, transfers, AdvancePayment,year.Next());
		Base::InsertSpecifiedTransfers(*accEnergy, transfers, Invoice,year.Next());
		
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
	constexpr static Name TypeIdentifier{"ChimneySweeper"}; 
	constexpr static const char* Identifier = "Sascha Schneider"; 
	inline static constexpr IBAN iban{"DE82660501011021592702"};	
};

struct LocalCommunity
{
	constexpr static const char* Identifier = "Gemeindekasse Dettenheim";	
	inline static constexpr IBAN iban{"DE12660623660000005703"};	
};

template<typename S, typename Server = LocalCommunity>
struct PropertyTax: public AncilliaryRentalCostItemBase<S, PropertyTax<S,Server>, ApartmentArea> 
{ 
	using Base = AncilliaryRentalCostItemBase<S, PropertyTax<S,Server>, ApartmentArea>; 
	constexpr static const char* Identifier = Server::Identifier;	
	inline static constexpr IBAN iban = Server::iban;
	constexpr static Name TypeIdentifier{"PropertyTax"}; 
	constexpr static Entry Cause{"Grundsteuer"}; 
	constexpr static const char* Invoice = "Rechnung/Abwasser"; 
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<typename Base::AccountType>(iban);
		auto transfers = account->GetTransferOf(Cause,year);		

		auto sum = Base::TotalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<ApartmentArea>();
		auto num = GetStage<S,ApartmentArea>().GetQuantity();
			
		Base::results->insert({year,typename Base::ResultType{std::move(transfers),num,denom,sum,year}});
		
		return (*Base::results)[year].Get();
	}
};

template<typename S, typename Server = LocalCommunity>
struct Sewage: public AncilliaryRentalCostItemBase<S, Sewage<S,Server>, WaterCounter>
{ 
	using Base = AncilliaryRentalCostItemBase<S, Sewage<S,Server>, WaterCounter>; 
	constexpr static const char* Identifier = Server::Identifier;	
	inline static constexpr IBAN iban = Server::iban;
	constexpr static Entry Cause{"Abschlag/Abwasser"}; 
	constexpr static Name TypeIdentifier{"Sewage"}; 
	constexpr static Entry Invoice{"Rechnung/Abwasser"}; 
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<typename Base::AccountType>(iban);
		auto transfers = account->GetTransferOf(Cause,year);		
		Base::InsertSpecifiedTransfers(*account, transfers, Invoice,year.Next());

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
//~ using CalculationItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>, Heating<S>>;

#endif
