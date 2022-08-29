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
	
	static void Calculate(const DateTimes::Year& year)
	{
		auto account = Bank::Get<AccountType>(Derived::iban);
		auto transfers = account[year];
		
		auto cwb = StageT::ColdWaterCounter::Instance().ConsumptionssBegin();
		Logger::Log()<<"WASSER"<<": "<<StageT::ColdWaterCounter::Instance().Number<<std::endl;
		//~ Logger::Log()<<"WASSER"<<": "<<*cwb<<std::endl;
		auto acc = TotalSum(transfers->cbegin(), transfers->cend());
		
		auto a = StageContainerType::Instance().GetTotal<Q>();
		auto b = GetStage<StageType,Q>().GetQuantity();
		results->insert({year,ResultType{std::move(transfers),std::move(QuantityRatio::Calculate(b,a,acc)),year}});
	}
	
	static const ResultType& Result(const DateTimes::Year& y){ return (*results)[y]; }
	static std::ostream& Display(std::ostream& os)
	{
		os<<results->cbegin()->first<<" Result "<<std::endl;
		return os;
	}
protected:
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
	inline static const IBAN ibanGas{"DE68600501010002057075"};	
	inline static const IBAN ibanEnergy{"DE56600501017402051588"};	
	
	static void Calculate(const DateTimes::Year& year)
	{
		auto accGas = Bank::Get<typename Base::AccountType>(ibanGas);
		auto transfers = accGas[year];
		auto accEnergy = Bank::Get<typename Base::AccountType>(ibanEnergy);
		auto transfersEnergy = accEnergy[year];
		
		transfers->insert(transfers->end(), transfersEnergy->begin(), transfersEnergy->end());
		
		auto acc = Base::TotalSum(transfers->cbegin(), transfers->cend());
				
		auto a = StageContainerType::Instance().GetTotal<HeatingProportion>();
		auto b = GetStage<S,HeatingProportion>().GetQuantity();
		Base::results->insert({year,typename Base::ResultType{std::move(transfers),std::move(QuantityRatio::Calculate(b,a,acc)),year}});
	}
};


template<typename S>
struct ChimneySweeper: AncilliaryRentalCostItemBase<S,ChimneySweeper<S>, IndividualUnit> 
{ 
	constexpr static const char* Name = "ChimneySweeper"; 
	constexpr static const char* Identifier = "Sascha Schneider"; 
	inline static const IBAN iban{"DE82660501011021592702"};	
};

template<typename S, typename Derived, typename Q>
struct LocalCommunity: public AncilliaryRentalCostItemBase<S, Derived, Q>
{
	using Base =  AncilliaryRentalCostItemBase<S,LocalCommunity<S,Derived,Q>, Derived>;
	constexpr static const char* Identifier = "Gemeindekasse Dettenheim";	
	inline static const IBAN iban{"DE12660623660000005703"};	
};

template<typename S>
struct PropertyTax: public LocalCommunity<S, PropertyTax<S>, ApartmentArea> 
{ 
	constexpr static const char* Name = "PropertyTax"; 
	constexpr static const char* CauseString = "Grundsteuer"; 
};

template<typename S>
struct Sewage: public LocalCommunity<S, Sewage<S>, ApartmentArea> 
{ 
	using Base = LocalCommunity<S, Sewage<S>, ApartmentArea>; 
	constexpr static const char* CauseString = "Abschlag/Abwasser"; 
	constexpr static const char* Name = "Sewage"; 
	constexpr static const char* InvoiceString = "Rechnung/Abwasser"; 
	
	//~ static void Calculate(const DateTimes::Year& year)
	//~ {
		//~ auto account = Bank::Get<typename Base::AccountType>(Base::iban);
		//~ auto transfers = account[year];
		
		//~ auto acc = Base::Base::TotalSum(transfers->cbegin(), transfers->cend());
		//~ auto cwb = S::ColdWaterCounter::Instance().ConsumptionssBegin();
		//~ auto hwb = S::HotWaterCounter::Instance().ConsumptionssBegin();
		
		
		//~ auto a = StageContainerType::Instance().GetTotal<ApartmentArea>();
		//~ auto b = GetStage<S,ApartmentArea>().GetQuantity();
		//~ Base::Base::results->insert({year,typename Base::Base::ResultType{std::move(transfers),std::move(QuantityRatio::Calculate(b,a,acc)),year}});
	//~ }
};

template<typename S>
using CalculationItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>, BuildingCleaning<S>, Heating<S>>;

#endif
