#include "../String/String_.hpp"
#include "../To/To.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.hpp"
#include "../Common/DateTimes.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/ToQuantity.hpp"
#include "../CSV/Elements.hpp"
#include "../Components/IOComponents.hpp"
#include "../Components/ParseComponents.hpp"
#include "../Components/Interfaces.hpp"
#include "../Common/Configuration.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../File/RepositoryValue.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "Reading.hpp"
#include "ReadingContainer.hpp"
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#pragma once

//~ template<typename T, typename A> class Analyzer;

template<typename ConfigT>
class Counter
{
public:
	using Config = ConfigT;
	using MeterType = Config::MeterT;
	using ReadingType = Reading<typename Config::Unit>;
	using QuantityType = ReadingType::QuantityType;
	using DateType = ReadingType::DateType;
	using DataType = std::shared_ptr<ReadingType>;
	using ResultValueType = FS::CounterValue<DataType>;
	using InType = InImpl<Counter<Config>>;
	using OutType = OutImpl<Counter<Config>>;
	using ParseType = ParseImpl<Counter<Config>>;
	using IOType = IO<Counter<Config>>;
	using ReadingContainerType = ReadingContainer<DataType>;
	using Type = MeterType;
	using CounterType = Counter<ConfigT>;
	using Unit = Config::Unit;
	using CIteratorReading = std::vector<DataType>::const_iterator;
	using InputIterator = std::vector<std::string>::const_iterator;
	inline static const uint Number = Config::Number;
	inline static constexpr const char TypeIdentifier[] = "Counter";
	inline static const std::string Identifier = Config::CounterName;

	static Counter& Instance()
	{
		static Counter instance;
		return instance;
	}
	
	template<typename Separator = T::char_<'\t'>>
	static std::ostream& Display(std::ostream& out)	{	return (*io)(out);	}
	
	constexpr const char* GetName(){ return name; }
	std::string GetFileName(){	return std::string(name) + std::string(".ctrv");	}

	template<typename Cont>
	void RegisterTo(Cont& cont) { cont.insert(std::make_pair(Instance().GetFileName(),  typename Cont::mapped_type(Instance().GetFileName(), Identifier,
		[&](InputIterator begin, InputIterator end){ Parse(begin,end); }, 
		[&](const std::string& s){ return GetValue(s); }, 
		[&](InputIterator begin, InputIterator end){ return Update(begin,end); },
		[&](std::ostream& s){ Display(s); }
		)));	}

	template<typename Op>
	decltype(auto) Get(const Op&& op){ return op(readings->CBegin(),readings->CEnd());	}
	decltype(auto) GetValue(const std::string& s) { return std::make_unique<FS::CounterValue<DataType>>(*(readings->CBegin())); }
	decltype(auto) ReadingsBegin() { return readings->CBegin(); }
	decltype(auto) ReadingsEnd() { return readings->CEnd(); }		

	void Parse(InputIterator begin, InputIterator end) {	(*iParse)(begin,end);	}
	bool Exec(std::istream& is, std::ostream& os) { return AddReading(is,os);}
	bool AddReading(std::istream& is, std::ostream& os) 
	{
		(*io)(std::cin);
		(*io)(std::cout);
		(*io)(std::cin);
		auto r = readings->CBegin();
		auto p = *(*r);
		os<<QuantityType::Identifier<<"\n";
		auto q = ToQuantity<QuantityType>(is,p.Value());
		auto date = DateTimes::Date::Create(is, os);
		auto n = std::make_shared<ReadingType>(q, date);
		os<<*n<<std::endl;
		//assert(p<*n);

		//addReading(n);

		return true;		 
	};
	bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in",*(begin), *(++begin)); return true; }
private:
	friend InType;
	friend OutType;
	friend ParseType;
	template<typename T, typename A> friend class Analyzer;
	
	void write(const std::string sourcePath = ".")
	{
		Logger::Log<Info>()<<"Write Counter: "<<GetName()<<" to: "<<csv->GetDestinationPath()<<std::endl;
		csv->Write<CounterType>();
	}
	
	void add(InputIterator begin, InputIterator end) 
	{ 
		auto s = (*begin);
		auto s2 = *(begin+2);
		auto valueResult = TryTo<double>(s2);
		Logger::Log("Add in",s,s2,valueResult);  
		
		if(valueResult.Valid)
		{
			auto reading = ReadingType::Create(s,s2);
			addReading(reading);
			write();
		}			
	}
	
	void addReading(DataType reading)
	{			
		readings->Add(reading);
		Logger::Log<Info>("Reading added",name, *reading);
	}
	
	std::unique_ptr<std::ofstream> write(std::unique_ptr<std::ofstream> of)	{	return (*io)(std::move(of));	}
	
	inline static const char* name = Config::CounterName.c_str();	
	inline static std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>> readings = std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>>(new ReadingContainerType(),DebugDeleter<ReadingContainerType>());
	inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(std::string(Configuration::Repository::SourcePath) + "/" + std::string(name) )));
	inline static std::unique_ptr<FS::CSV> csv = std::make_unique<FS::CSV>(std::move(fileInfo));
	inline static std::unique_ptr<IOType> io = std::make_unique<IOType>(std::make_unique<InType>(), std::make_unique<OutType>());
	inline static std::unique_ptr<IParse> iParse = std::make_unique<ParseType>();
	
	template<typename Iterator>
	static DataType createReading(Iterator cbegin, Iterator cend)
	{
		if(cbegin != cend)
		{
			try 
			{
				return ReadingType::Create(*cbegin,*(cbegin+1));
			} 
			catch (const std::exception& e) 
			{
				Logger::Log<Error>()<<"Error: CreateReading"<<e.what()<<std::endl;
			}		
		}
		else
			Logger::Log<Error>()<<"Error: CreateReading-> Not enough values"<<std::endl;
			
		return  DataType(new ReadingType(QuantityType(0.0), DateType(Parsers::Parser<std::string,DateType>::Parse("01.01.2000"))));
	}
	
	Counter()	{ 	Logger::Log<Info>()<<"Initialize Counter: "<<MeterType::Name<<"_"<<Config::Number<<std::endl; 	Config::Unit::Instance(); };
	~Counter()	{ };
	Counter& operator=(const Counter&) = delete;
	Counter(const Counter& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Counter<C>& c){	return c.Display(strm);}
