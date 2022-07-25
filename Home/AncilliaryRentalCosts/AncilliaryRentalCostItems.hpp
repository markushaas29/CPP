#include <tuple>
#include "../Stage.hpp"
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

template<typename Derived, typename Q>
struct AncilliaryRentalCostItemBase
{
	using Type = Derived;
	using StageQuantity = Q;
	constexpr static const char* Name = "";//Derived::Name; 
	static void Calculate()
	{
		auto s = Bank::Get<Bank::Raiba<0>>(Derived::iban);
		auto t = s[Derived::iban];
		auto q = GetTransfer<Quantity<Sum>>(*(t[0]));
		auto a = StageContainerType::Instance().GetTotal<Q>();
		std::cout<<"AAC::::::::::: "<<a<<std::endl;
		std::cout<<"AAC::::::::::: "<<GetStage<Middle,Q>().GetQuantity()<<std::endl;
		auto b = GetStage<Middle,Q>().GetQuantity();
		auto c = b / a;
		auto d = q * c;
		std::cout<<"AAC::::::::::: "<<c<<"\t"<<d<<"\t"<<q<<std::endl;
	}
};

struct BuildingInsurance: AncilliaryRentalCostItemBase<BuildingInsurance, IndividualUnit> 
{ 
	constexpr static const char* Name = "BuildingInsurance"; 
	constexpr static const char* Identifier = "SV Gebaeudeversicherung"; 
	inline static const IBAN iban{"DE97500500000003200029"};	
};

struct WasteFees: AncilliaryRentalCostItemBase<WasteFees, Persons> 
{ 
	constexpr static const char* Name = "WasteFees"; 
	constexpr static const char* Identifier = "Abfallwirtschaftsbetrieb"; 
	inline static const IBAN iban{"DE44600501010008017284"};	
};

struct ChimneySweeper: AncilliaryRentalCostItemBase<ChimneySweeper, IndividualUnit> 
{ 
	constexpr static const char* Name = "ChimneySweeper"; 
	constexpr static const char* Identifier = "Sascha Schneider"; 
	inline static const IBAN iban{"DE82660501011021592702"};	
};

template<typename Derived, typename Q>
struct LocalCommunity: AncilliaryRentalCostItemBase<Derived, Q>
{
	constexpr static const char* Identifier = "Gemeindekasse Dettenheim";	
	inline static const IBAN iban{"DE12660623660000005703"};	
};

struct PropertyTax: public LocalCommunity<PropertyTax, ApartmentArea> 
{ 
	constexpr static const char* Name = "PropertyTax"; 
	constexpr static const char* CauseString = "Grundsteuer"; 
};

struct Sewage: public LocalCommunity<Sewage, ApartmentArea> 
{ 
	constexpr static const char* Name = "Sewage"; 
	constexpr static const char* CauseString = "Abschlag/Abwasser"; 
	constexpr static const char* InvoiceString = "Rechnung/Abwasser"; 
};

using CalculationItems = std::tuple<BuildingInsurance,WasteFees,ChimneySweeper,Sewage,PropertyTax>;

#endif
