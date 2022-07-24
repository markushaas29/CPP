#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "Stage.hpp"
#include "AncilliaryRentalCosts/AncilliaryRentalCostsContainer.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../File/Account.hpp"
#include "../File/Repository.hpp"
#include "../File/FileSystem.hpp"
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef HOUSE_HPP
#define HOUSE_HPP

using HouseQuantitiesType = std::tuple<ApartmentArea,Rooms,IndividualUnit,Persons>;

struct HouseConfiguration
{
	inline static constexpr const char* AccountName = "Umsaetze_DE19660623660009232702.csv";
	inline static constexpr const char* CsvRepo = "/home/markus/Downloads/CSV_Files";
};

//~ template<typename>	class House;
	
//~ template<typename T, typename C>
//~ const ItemT& GetTransfer(Transfer<Account,TupleT>const& t){		return std::get<ItemT>(t.transferItems);	};

template<typename ConfigT = HouseConfiguration>
class House
{
public:
	using Type = House<ConfigT>;
	using Configuration = ConfigT;
	inline static constexpr const char* Name = "House";
	inline static constexpr const char* AccountName = HouseConfiguration::AccountName;
	inline static constexpr const char* CSVRepository = HouseConfiguration::CsvRepo;
	
	static House& Instance()
	{
		static House instance;
		return instance;
	}
	
	std::ostream& Display(std::ostream& os) {	return os;	}
	
	void Calculate() {	StageContainerType::Instance().Calculate();	}
	
private:
	HouseQuantitiesType quantities; 
	
	House(): quantities(0, 0, 0, ""){ 
		Logger::Log()<<"CTOR: "<<Name<<std::endl;
		std::string csv = "/home/markus/Downloads/CSV_Files";
		CSV::Repository::Map(csv);
		CSV::Repository::Register();
		CSV::Repository::ParseAll();
	
		auto outFile = std::ofstream("out.txt");
		auto inFile = std::ofstream("//home//markus//Downloads//in.txt");
		auto keyFileN = std::string("//home//markus//Downloads//Keys.txt");
		auto keys = FileSystem::ReadLines(keyFileN);
		
		StageContainerType::Instance().Display(std::cout);
		AncilliaryRentalCostsContainer<Top>::Instance();
		//~ CounterContainerType::Instance().Display(std::cout);
	}
	
	template<typename T>
	decltype(auto) Get(){	return StageContainerType::Instance().GetTotal<T>();	};
	
	//~ template <size_t I = 0, typename... Ts>
	//~ constexpr std::tuple<Ts...> SumStageQuantities(std::tuple<Ts...>& t1)
	//~ {
	    //~ if constexpr(I == sizeof...(Ts))
			//~ return t1;
	    //~ else {
			//~ using TR = decltype(std::get<I>(quantities));
			//~ using T = std::remove_reference<TR>::type;
			//~ auto tq = T(StageContainerType::Instance().GetTotal<T>());
			
			//~ std::get<I>(quantities) = tq;
			
	        //~ return SumStageQuantities<I + 1>(t1);
	    //~ }
	//~ }
	
	~House()	{ Logger::Log()<<"Destructor"<<std::endl; }
	House& operator=(const House&) = delete;
	House(const House& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const House<C>& c)
{
	return c.Display(strm);
}

#endif
