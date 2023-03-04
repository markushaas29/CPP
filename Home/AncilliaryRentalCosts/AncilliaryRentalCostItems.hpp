#include <tuple>
#include <memory>
#include "AncilliaryRentalCostResult.hpp"
#include "../Stage.hpp"
#include "../StageContainer.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../File/Info.hpp"
#include "../../File/Node.hpp"
#include "../../Wrapper/Wrapper.hpp"
#include "../../Home/StageQuantities.hpp"
#include "../../File/Raiffeisenbank.hpp"
#include "../../File/Account.hpp"
#include "../../File/AccountQuery.hpp"
#include "../../Common/Configuration.hpp"

#ifndef ANCILLIARYRENTALCOST_HPP
#define ANCILLIARYRENTALCOST_HPP

template<typename StageT,typename Derived, typename Q>
struct AncilliaryRentalCostItemBase
{
	using Type = Derived;
	using StageType = StageT;
	using StageQuantity = Q;
	using StageQuantityType =typename StageQuantity::TQuantity;
	using AccountType = Configuration::AncilliaryRentalCosts::AccountType;
	using RatioType =  ItemFraction<StageQuantityType>;
	using ResultType =  AncilliaryRentalCostItemResult<Derived,RatioType,StageType,StageQuantity,AccountType>;
	using ContType = std::vector<ResultType>;
	constexpr static Name TypeIdentifier = Name{""};
	
	static decltype(auto) Calculate(const DateTimes::Year& year)
	{
		auto transfers = getTransfers(year,Derived::iban);
		
		auto sum = totalSum(transfers->cbegin(), transfers->cend());
		
		auto denom = StageContainerType::Instance().GetTotal<Q>();
		auto num = GetStage<StageType,Q>().GetQuantity();
		quantityRatio = RatioType{num,denom,sum};
		insert(ResultType{std::move(transfers),std::move(quantityRatio),year});
		
		return get(year).Get();
	}
	
	static const ResultType& Result(const DateTimes::Year& y){ return get(y); }
	static std::ostream& Display(std::ostream& os){	return os<<results->cbegin()->first<<" Result "<<std::endl;	}
protected:
	static inline RatioType quantityRatio{StageQuantityType{0},StageQuantityType{1},Quantity<Sum>(0)};
	static decltype(auto) getTransfers(const DateTimes::Year& year, const IBAN& iban)
	{
		auto account = Bank::Get<AccountType>(Derived::iban);
		typename ResultType::TransfersPtr transfers;

		if(account)
		{
			transfers = (*account)[year];
		}
		else
		{
			transfers = std::make_unique<typename ResultType::Transfers>();
			Logger::Log<Error>("No transfers for ", Derived::Identifier);
		}
		
		return transfers;
	}
	
	template <typename Acc, typename T, typename... Categories>
	static decltype(auto) insertSpecifiedTransfers(const Acc& account, std::unique_ptr<T>& transfers, Categories... categories)
	{ 
		auto specifiedtransfers = account.GetTransferOf(categories...);
		
		if(specifiedtransfers->begin() != specifiedtransfers->end())
			transfers->insert(transfers->end(), specifiedtransfers->begin(), specifiedtransfers->end());
		else
			Logger::Log<Info>("No Transfers found for categories: ", categories...);
	}
	
	template<typename It>
	static decltype(auto) totalSum(It begin, It end)
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

	static const ResultType& get(const DateTimes::Year& y)
	{
		auto it = std::find_if(results->cbegin(), results->cend(),[&y](const auto& r){ return r.Year()==y;});
		Logger::Log<Info>("Get: ", y);
	Logger::Log<Info>("GET: \n", results->size()); it->Display(std::cout);		
		if(it != results->cend())
			return *it;
		Logger::Log<Error>("No result found for year: ",y, Logger::Source(""));
		throw std::invalid_argument(y.ToString());
	}
	static decltype(auto) insert(ResultType&& r) { results->push_back(std::move(r)); Logger::Log<Info>("Insert: \n", results->size()); results->at(0).Display(std::cout);}
private:
	inline static std::unique_ptr<ContType> results = std::make_unique<ContType>();
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
		auto transfers = Base::getTransfers(year,iban);		
		auto sum = Base::totalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<IndividualUnit>() + Quantity<Scalar>(1);
		auto num = GetStage<S,IndividualUnit>().GetQuantity();
		
		if constexpr (std::is_same<S,Top>::value)
			num = num + Quantity<Scalar>(1);
		
		Base::quantityRatio = typename Base::RatioType{num,denom,sum};
		Base::insert(typename Base::ResultType{std::move(transfers),std::move(Base::quantityRatio),year});
		
		return Base::get(year).Get();
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
		auto aq = Bank::AccountQuery<typename Base::AccountType,DateTimes::Year>{ibanGas,year};
		auto r = aq.Execute();
		Logger::Log<Info>("QUERY: ", r->size());
		auto accGas = Bank::Get<typename Base::AccountType>(ibanGas);
		auto transfers = accGas->GetTransferOf(AdvancePayment,year);
		Base::insertSpecifiedTransfers(*accGas, transfers, Invoice,year.Next());
		
		auto accEnergy = Bank::Get<typename Base::AccountType>(ibanEnergy);
		Base::insertSpecifiedTransfers(*accEnergy, transfers, AdvancePayment,year.Next());
		Base::insertSpecifiedTransfers(*accEnergy, transfers, Invoice,year.Next());
		
		auto sum = Base::totalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<HeatingProportion>();
		auto num = GetStage<S,HeatingProportion>().GetQuantity();
		Base::quantityRatio = typename Base::RatioType{num,denom,sum};
		Base::insert(typename Base::ResultType{std::move(transfers),std::move(Base::quantityRatio),year});
		
		return Base::get(year).Get();
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

		auto sum = Base::totalSum(transfers->cbegin(), transfers->cend());
				
		auto denom = StageContainerType::Instance().GetTotal<ApartmentArea>();
		auto num = GetStage<S,ApartmentArea>().GetQuantity();
			
		Base::quantityRatio = typename Base::RatioType{num,denom,sum};
		Base::insert(typename Base::ResultType{std::move(transfers),std::move(Base::quantityRatio),year});
		
		return Base::get(year).Get();
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
		Base::insertSpecifiedTransfers(*account, transfers, Invoice,year.Next());

		auto sum = Base::totalSum(transfers->cbegin(), transfers->cend());
		auto stageColdWater = S::ColdWaterCounter::Instance().Get(AnnualConsumption(year));
		auto stageHotWater = S::HotWaterCounter::Instance().Get(AnnualConsumption(year));
		
		auto houseHotWater = StageContainerType::Instance().GetCounterTotal<Hot>(AnnualConsumption(year));
		auto houseColdWater = StageContainerType::Instance().GetCounterTotal<Cold>(AnnualConsumption(year));
		
		Logger::Log<Info>("AnnualConsumption Sewage: \n",S::StageName,"\nStage cold" ,stageColdWater,"\nStage Hot",stageHotWater,"\nHouse Cold" ,houseColdWater,"\nHouse Hot",houseHotWater);
		auto stageWater = stageColdWater + stageHotWater;
		auto houseWater = houseHotWater + houseColdWater;

		Base::quantityRatio = typename Base::RatioType{stageWater.Get(),houseWater.Get(),sum};
		Base::insert(typename Base::ResultType{std::move(transfers),std::move(Base::quantityRatio),year});
		
		return Base::get(year).Get();
	}
};

template<typename S>
using CalculationItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>, BuildingCleaning<S>, Heating<S>>;

#endif
