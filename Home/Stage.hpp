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

using ApartmentQuantitiesType = std::tuple<ApartmentArea,Rooms,IndividualUnit,Name,Persons, HeatingProportion, BuildingCleaningProportion, Advance,MonthlyRent,IncidentalHeatingCosts,GarageRental>;
inline static constexpr size_t ApartmentQuantitiesCount = std::tuple_size_v<ApartmentQuantitiesType>;
using StageMap = std::map<std::string, std::string>;
using YearDataMapKeyType = uint;

template<typename T>
using YearDataMapValueType = std::shared_ptr<YearDatas<T>>;

template<typename T>
using YearDataMap = std::map<YearDataMapKeyType, YearDataMapValueType<T>>;

template<typename> class Stage;
	
template<typename S, typename ItemT>
const ItemT& GetStage(){ return std::get<ItemT>( S::Instance().quantities );	};

template<typename List>
class StageContainer{};

template<typename ConfigT>
class Stage
{
	template<typename, typename ItemT>
	friend const ItemT& GetStage();
	
	template<typename List>
	friend class StageContainer;	
public:
	using Type = Stage<ConfigT>;
	using Configuration = ConfigT;
	using YearDataMapType = YearDataMapValueType<ConfigT>;
	using YearDataMapPtrType = std::unique_ptr<YearDataMapType>;
	
	using ColdWaterCounter = ConfigT::ColdWaterCounter;
	using HotWaterCounter = ConfigT::HotWaterCounter;
	using EnergyCounter = ConfigT::EnergyCounter;
	inline static constexpr int Number = Configuration::Number;
	inline static constexpr const char* StageName = Configuration::Name;
	
	YearDataMapType operator[](YearDataMapKeyType key) { return 0; }
	
	static Stage& Instance()
	{
		static Stage instance = Stage(Map);
		return instance;
	}
	
	static void Set(const StageMap& m)	{	Map = m;	}
	
	std::ostream& Display(std::ostream& os) 
	{
		os<<"Stage: "<<Type::StageName<<"\nName: "<<std::get<Name>(quantities)<<"\tUnits: "<<std::get<IndividualUnit>(quantities)<<"\tRooms:"<<std::get<Rooms>(quantities)<<"\tArea: "<<std::get<ApartmentArea>(quantities)<<"\tPersons: "<<std::get<Persons>(quantities);
		os<<"\tHeating Cost: "<<std::get<HeatingProportion>(quantities)<<"\tBuildingCleaning: "<<std::get<BuildingCleaningProportion>(quantities);
		return os<<"MonthlyRent: "<<std::get<MonthlyRent>(quantities)<<"\tAdvance: "<<std::get<Advance>(quantities)<<"\tGarage: "<<std::get<GarageRental>(quantities)<<"\tIncidentalHeatingCosts: "<<std::get<IncidentalHeatingCosts>(quantities)<<std::endl;
	}
	
private:
	inline static StageMap Map = StageMap();
	inline static YearDataMapPtrType yearData = YearDataMapPtrType(new YearDataMapType()); 
	ApartmentQuantitiesType quantities;
	
	Stage(){ }
	Stage(const StageMap& m): 
		quantities(Configuration::Area,Configuration::Rooms,Configuration::Units,m.at(Name::Identifier),m.at(Persons::Key),Configuration::Heating,Configuration::BuildingCleaning,m.at(Advance::Key),m.at(MonthlyRent::Key),m.at(IncidentalHeatingCosts::Key),m.at(GarageRental::Key)){ 	}
	
	~Stage()	{  }
	Stage& operator=(const Stage&) = delete;
	Stage(const Stage& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Stage<C> c){	return c.Display(strm);}

using Top = Stage<TopConfiguration>;
using Middle = Stage<MiddleConfiguration>;
using Bottom = Stage<BottomConfiguration>;

#endif
