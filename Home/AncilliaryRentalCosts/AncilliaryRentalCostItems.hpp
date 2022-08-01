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
	
	static void Calculate(const DateTimes::Year& y)
	{
		auto s = Bank::Get<AccountType>(Derived::iban);
		auto t = s[y];
		auto q = GetTransfer<Quantity<Sum>>(*((*t)[0]));
		auto a = StageContainerType::Instance().GetTotal<Q>();
		std::cout<<StageType::Name<<"\t Total "<<a<<"\t Stage"<<GetStage<StageType,Q>().GetQuantity()<<std::endl;
		auto b = GetStage<StageType,Q>().GetQuantity();
		auto c = b / a;
		auto d = q * c;
		auto qr = QuantityRatio::Calculate(b,a,q);
		results->insert({y,ResultType{std::move(t),y}});
	}
	
	static const ResultType& Result(const DateTimes::Year& y){ return (*results)[y]; }
	static std::ostream& Display(std::ostream& os)
	{
		os<<results->cbegin()->first<<" Result "<<std::endl;
		return os;
	}
private:
	inline static std::unique_ptr<MapType> results = std::make_unique<MapType>();	
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
struct ChimneySweeper: AncilliaryRentalCostItemBase<S,ChimneySweeper<S>, IndividualUnit> 
{ 
	constexpr static const char* Name = "ChimneySweeper"; 
	constexpr static const char* Identifier = "Sascha Schneider"; 
	inline static const IBAN iban{"DE82660501011021592702"};	
};

template<typename S, typename Derived, typename Q>
struct LocalCommunity: AncilliaryRentalCostItemBase<S, Derived, Q>
{
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
	constexpr static const char* Name = "Sewage"; 
	constexpr static const char* CauseString = "Abschlag/Abwasser"; 
	constexpr static const char* InvoiceString = "Rechnung/Abwasser"; 
};

template<typename S>
using CalculationItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>>;

#endif
