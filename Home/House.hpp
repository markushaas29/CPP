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
#include "../Calculator/CalculatorConfiguration.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
//~ #include "UtilitiesStatement.hpp"
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef HOUSE_HPP
#define HOUSE_HPP

struct HouseConfiguration
{
	inline static const char* AccountName = "Umsaetze_DE19660623660009232702.csv";
	inline static const char* CsvRepo = "/home/markus/Downloads/CSV_Files";
};

template<typename ConfigT = HouseConfiguration>
class House
{
public:
	using Type = House<ConfigT>;
	using Configuration = ConfigT;
	inline static const char* Name = "House";
	inline static const char* AccountName = HouseConfiguration::AccountName;
	inline static const char* CSVRepository = HouseConfiguration::CsvRepo;
	
	static House& Instance()
	{
		static House instance;
		return instance;
	}
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
	void Calculate() 
	{
		StageContainerType::Instance().Calculate();
	}
	
private:
	
	House(){ 
		Logger::Log()<<"CTOR: "<<Name<<std::endl;
		std::string csv = "/home/markus/Downloads/CSV_Files";
		CSV::Repository::Map(csv);
		CSV::Repository::Register();
		CSV::Repository::ParseAll();
	
		auto outFile = std::ofstream("out.txt");
		auto inFile = std::ofstream("//home//markus//Downloads//in.txt");
		auto keyFileN = std::string("//home//markus//Downloads//Keys.txt");
// 		auto out = Transfers<Bank::Raiba<0>>();
// 		auto in = Transfers<Bank::Raiba<0>, Bank::In>();
// 		std::cout<<"\n-------------------------------------------------------- Transfers IN --------------------------------------------------------------"<<std::endl;
// 		auto raibaOut = Transfers<Bank::Raiba<0>, Bank::Out>();
// 		auto rbo = raibaOut();
		auto keys = FileSystem::ReadLines(keyFileN);
// 		rbo.Display(inFile,keys);
		
		StageContainerType::Instance().Display(std::cout);
		CounterConatinerType::Instance().Display(std::cout);
	}
	
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
