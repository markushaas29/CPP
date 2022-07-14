#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Home/StageQuantities.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef CALCULATORCONFIGURATION_HPP
#define CALCULATORCONFIGURATION_HPP


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
};


struct BuildingInsurance: AncilliaryRentalCostItemBase<BuildingInsurance, IndividualUnit> 
{ 
	constexpr static const char* Name = "BuildingInsurance"; 
	constexpr static const char* KeyString = "SV Gebaeudeversicherung"; 
};

struct WasteFees: AncilliaryRentalCostItemBase<WasteFees, Persons> 
{ 
	constexpr static const char* Name = "WasteFees"; 
	constexpr static const char* KeyString = "Abfallwirtschaftsbetrieb"; 
};

struct ChimneySweeper: AncilliaryRentalCostItemBase<ChimneySweeper, IndividualUnit> 
{ 
	constexpr static const char* Name = "ChimneySweeper"; 
	constexpr static const char* KeyString = "Sascha Schneider"; 
};


template<typename Derived, typename Q>
struct LocalCommunity: AncilliaryRentalCostItemBase<Derived, Q>
{
	constexpr static const char* KeyString = "Gemeindekasse Dettenheim";	
};

struct PropertyTax: LocalCommunity<PropertyTax, ApartmentArea> 
{ 
	constexpr static const char* Name = "PropertyTax"; 
	constexpr static const char* CauseString = "Grundsteuer"; 
};

struct Sewage: AncilliaryRentalCostItemBase<Sewage, ApartmentArea> 
{ 
	constexpr static const char* Name = "Sewage"; 
	constexpr static const char* CauseString = "Abschlag/Abwasser"; 
	constexpr static const char* InvoiceString = "Rechnung/Abwasser"; 
};

#endif
