#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Parser.hpp"
#include "YearDatas.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "StageConfiguration.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <tuple>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef STAGE_HPP
#define STAGE_HPP

using StageMap = std::map<std::string, std::string>;
using YearDataMapKeyType = uint;
template<typename T>
using YearDataMapValueType = std::shared_ptr<YearDatas<T>>;
template<typename T>
using YearDataMap = std::map<YearDataMapKeyType, YearDataMapValueType<T>>;

template<typename ConfigT>
class Stage
{
public:
	using Type = Stage<ConfigT>;
	using Configuration = ConfigT;
	using YearDataMapType = YearDataMapValueType<ConfigT>;
	using YearDataMapPtrType = std::unique_ptr<YearDataMapType>;
	
	using ColdWaterCounter = ConfigT::ColdWaterCounter;
	using HotWaterCounter = ConfigT::HotWaterCounter;
	using EnergyCounter = ConfigT::EnergyCounter;
	static constexpr int Number = Configuration::Number;
	inline static const char* Name = Configuration::Name;
	
	const Quantity<Area> AreaQuantity() { return this->area.Get(); }
	const Quantity<Area> RoomsQuantity() { return this->rooms.Get(); }
	const Quantity<Scalar> UnitsQuantity() { return this->individualUnit.Get(); }
	const Quantity<Scalar> PersonsQuantity() { return this->persons.Get(); }
	const Quantity<Sum> AdvanceQuantity() { return this->advance.Get(); }
	const Quantity<Sum> IncidentalHeatingCostsQuantity() { return this->incidentalHeatingCosts.Get(); }
	const Quantity<Sum> MonthlyRentQuantity() { return this->monthlyRent.Get(); }
	const Quantity<Sum> GarageRentalQuantity() { return this->garageRental.Get(); }
// 	YearDataMapType operator[](YearDataMapKeyType key) { return 0; }
	YearDataMapType operator[](YearDataMapKeyType key) { return 0; }
	
	static Stage& Instance()
	{
		static Stage instance = Stage(Map);
		return instance;
	}
	
	static void Set(const StageMap& m)	{	Map = m;	}
	
	std::ostream& Display(std::ostream& os) 
	{
		os<<"Name: "<<Type::Name<<"\tUnits:"<<individualUnit.Get()<<"\tRooms:"<<rooms.Get()<<"\tArea: "<<area.Get()<<"\tPersons: "<<persons.Get();
		return os<<"MonthlyRent: "<<monthlyRent.Get()<<"\tAdvance: "<<advance.Get()<<"\tGarage: "<<garageRental.Get()<<"\tIncidentalHeatingCosts: "<<incidentalHeatingCosts.Get()<<std::endl;
	}
	
private:
	inline static StageMap Map = StageMap();
	inline static YearDataMapPtrType yearData = YearDataMapPtrType(new YearDataMapType()); 
	std::tuple<ApartmentArea,Rooms,IndividualUnit,Persons,Advance,MonthlyRent,IncidentalHeatingCosts,GarageRental> quantities;
	
	ApartmentArea area = ApartmentArea(Configuration::Area);
	Rooms rooms = Rooms(Configuration::Rooms);
	IndividualUnit individualUnit = IndividualUnit(Configuration::Units);
	Persons persons;
	Advance advance;
	IncidentalHeatingCosts incidentalHeatingCosts;
	GarageRental garageRental;
	MonthlyRent monthlyRent;
	
	Stage(){ Logger::Log()<<"CTOR: "<<Number<<std::endl;}
	Stage(const StageMap& m): 
		quantities(Configuration::Area,Configuration::Rooms,Configuration::Units,m.at(Persons::Key),m.at(Advance::Key),m.at(MonthlyRent::Key),m.at(IncidentalHeatingCosts::Key),m.at(GarageRental::Key)),
		persons(m.at(Persons::Key)),
		advance(m.at(Advance::Key)),
		monthlyRent(m.at(MonthlyRent::Key)),
		incidentalHeatingCosts(m.at(IncidentalHeatingCosts::Key)),
		garageRental(m.at(GarageRental::Key))
	{ 
// 		for(auto kv : m)
// 			Logger::Log()<<kv.first<<": "<<kv.second<<std::endl;
// 		Logger::Log()<<"CTOR MAP: "<<Number<<std::endl;
	}
	
	~Stage()	{ Logger::Log()<<"Destructor"<<std::endl; }
	Stage& operator=(const Stage&) = delete;
	Stage(const Stage& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Stage<C> c){	return c.Display(strm);}

template<typename T, typename Q>
struct GetQuantity{};

template<typename T>
struct GetQuantity<T, ApartmentArea>
{
	static Quantity<ApartmentArea::Unit> Value() { return T::Instance().AreaQuantity(); }
};

template<typename T>
struct GetQuantity<T, Persons>
{
	static Quantity<Persons::Unit> Value() { return T::Instance().PersonsQuantity(); }
};

template<typename T>
struct GetQuantity<T, IndividualUnit>
{
	static Quantity<IndividualUnit::Unit> Value() { return T::Instance().UnitsQuantity(); }
};

using Top = Stage<TopConfiguration>;
using Middle = Stage<MiddleConfiguration>;
using Bottom = Stage<BottomConfiguration>;

#endif
