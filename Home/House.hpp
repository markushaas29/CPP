#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "Stage.hpp"
#include "AncilliaryRentalCosts/AncilliaryRentalCostsContainer.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
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
	void Calculate() {	StageContainerType::Instance().Calculate<AncilliaryRentalCostsContainer>();	}
private:
	HouseQuantitiesType quantities; 
	
	House(): quantities(0, 0, 0, ""){ 
		Logger::Log()<<"CTOR: "<<Name<<std::endl;
		CSV::Repository<CSV::CommonRepositoryConfiguration>::Instance();
	
		auto outFile = std::ofstream("out.txt");
		auto inFile = std::ofstream("//home//markus//Downloads//in.txt");
		
		StageContainerType::Instance().Display(std::cout);
		StageContainerType::Instance().Calculate<AncilliaryRentalCostsContainer>();
	}
	
	template<typename T>
	decltype(auto) Get(){	return StageContainerType::Instance().GetTotal<T>();	};

	~House()	{ Logger::Log()<<"Destructor"<<std::endl; }
	House& operator=(const House&) = delete;
	House(const House& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const House<C>& c){	return c.Display(strm); }

#endif
