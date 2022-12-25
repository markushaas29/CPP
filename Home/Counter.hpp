#include "../String/String_.hpp"
#include "../String/To/To.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Common/DateTimes.hpp"
#include "../Quantity/Quantity.h"
#include "../CSV/Elements.hpp"
#include "../Common/Configuration.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../File/RepositoryValue.hpp"
#include "../InputManager/InputVisitor.hpp"
#include "../InputManager/InputManager.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "Reading.hpp"
#include "ReadingContainer.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef COUNTER_H
#define COUNTER_H

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
	
	using ReadingContainerType = ReadingContainer<DataType>;
	using Type = MeterType;
	using CounterType = Counter<ConfigT>;
	using Unit = Config::Unit;
	using CIteratorReading = std::vector<DataType>::const_iterator;
	using InputIterator = std::vector<std::string>::const_iterator;
	inline static const uint Number = Config::Number;
	inline static constexpr const char* TypeIdentifier = "Counter";
	inline static const std::string Identifier = Config::CounterName;

	static Counter& Instance()
	{
		static Counter instance;
		return instance;
	}
	
	template<typename Separator = T::char_<'\t'>>
	static std::ostream& Display(std::ostream& out)
	{
		DisplayHeader<Separator>(out);		
		for(auto it = readings->CBegin(); it != readings->CEnd(); ++it)
		{
			(*it)->Display(out);
			out<<std::endl;
		}
		
		return out;
	}
	
	constexpr const char* GetName(){ return Name; }
	std::string GetFileName(){	return std::string(Name) + std::string(".ctrv");	}

	template<typename Cont>
	void RegisterTo(Cont& cont) { cont.insert(std::make_pair(Instance().GetFileName(),  typename Cont::mapped_type(Instance().GetFileName(), Identifier,
		[&](InputIterator begin, InputIterator end){ Parse(begin,end); }, 
		[&](const std::string& s){ return GetValue(s); }, 
		[&](InputIterator begin, InputIterator end){ return Update(begin,end); })));	
		
		InputManager<Counter>::Instance().Register(std::make_unique<InputVisitor>(Name, 
		[&](InputIterator b, InputIterator e) { add(b,e); },
		[&](std::unique_ptr<std::ofstream> of) { return input(std::move(of)); }));
		}

	template<typename Op>
	decltype(auto) Get(const Op&& op){ return op(readings->CBegin(),readings->CEnd());	}
	
	decltype(auto) GetValue(const std::string& s) { Logger::Log("GET in",Number); return std::make_unique<FS::CounterValue<DataType>>(*(readings->CBegin())); }
	decltype(auto) ReadingsBegin() { return readings->CBegin(); }
	decltype(auto) ReadingsEnd() { return readings->CEnd(); }		

	template<typename Separator = T::char_<'\t'>>
	static std::ostream& DisplayHeader(std::ostream& out)
	{
		for (auto& it : Header)
			out<<it.first<<Separator::Value<<it.second<<std::endl;
		
		return out;
	}

	void Parse(InputIterator begin, InputIterator end)
	{
		for(auto it = (begin + Header.size()); it != end; ++it)
		{
			auto v = csv->ExtractValues(*it);
			DataType reading = CreateReading(v.cbegin(), v.cend());
			addReading(reading);
		}
	}	

	bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in",*(begin), *(++begin)); return true; }
private:
	void write(const std::string sourcePath = ".")
	{
		Logger::Log<Info>()<<"Write Counter: "<<GetName()<<" to: "<<csv->GetDestinationPath()<<std::endl;
		csv->Write<CounterType>();
	}
	
	void add(InputIterator begin, InputIterator end) 
	{ 
		auto s = (*begin);
		auto s2 = *(begin+2);
		auto valueResult = String_::TryTo<double>(s2);
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
		Logger::Log<Info>("Reading added",Name, *reading);
	}
	
	std::unique_ptr<std::ofstream> input(std::unique_ptr<std::ofstream> of)
	{
		(*of)<<Name<<":"<<";;"<<Config::Unit::Sign()<<";"<<MeterType::Name<<";";
		if(readings->CBegin() != readings->CEnd())
			(*of)<<*(*(readings->CEnd() - 1));
		(*of)<<std::endl;
		
		return of;
	}

	static std::map<std::string, std::string> createHeader()
	{
		std::map<std::string, std::string> m;
		m["CounterName"] = Name;
		m["Number"] =  std::to_string(Config::Number),
		m["Type"] = MeterType::Name,
		m["SiUnit"] =  Config::Unit::SiUnit(),
		m["Unit"] =  Config::Unit::Sign();
        return m;
	}
	
	inline static const char* Name = Config::CounterName.c_str();
	
	inline static const std::map<std::string, std::string> Header = createHeader();	
	inline static std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>> readings = std::unique_ptr<ReadingContainerType, DebugDeleter<ReadingContainerType>>(new ReadingContainerType(),DebugDeleter<ReadingContainerType>());
	
	inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(std::string(Configuration::Repository::SourcePath) + "/" + std::string(Name) )));
	inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
	
	template<typename Iterator>
	static DataType CreateReading(Iterator cbegin, Iterator cend)
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
			
		//~ return ReadingType::Create("1,0","01.01.2000");
		return  DataType(new ReadingType(QuantityType(0.0), DateType(Parsers::Parser<std::string,DateType>::Parse("01.01.2000"))));
	}
	
	static void Calculate()	{	}	
	
	Counter()	{ 	Logger::Log<Info>()<<"Initialize Counter: "<<MeterType::Name<<"_"<<Config::Number<<std::endl; 	};
	~Counter()	{ };
	Counter& operator=(const Counter&) = delete;
	Counter(const Counter& c) = delete;
};

template<> inline const char* InputManager<Counter>::Filename = "Counter";

template<> 
void InputManager<Counter>::Parse(InputIterator begin, InputIterator end)
{
	Instance();
	try
	{
		auto it = begin;
		auto dateLine = String_::Split<T::char_<':'>>(*it);
		auto date = dateLine.cbegin() + 1 != dateLine.cend() ? dateLine[1] : "1.1.2022" ;
		++it;
		for(; it != end; ++it)
		{
			auto vals = String_::Split<T::char_<':'>>(*it);
			auto values = String_::Split<T::char_<';'>>(*(vals.cbegin()+1));
			values.insert(values.begin(), date);
			visitors->at(vals[0])->Add(values.cbegin(),values.cend());
		}
	}
	catch(const std::exception& e)
	{
		Logger::Log<Error>("InputManager<Counter>::Parse ",e.what());
	}
}

struct CurrentValue
{ 
	template<typename It>
	decltype(auto) operator()(It begin, It end) const { return **begin; }
};

struct Difference
{ 
	template<typename It>
	decltype(auto) operator()(It begin, It end) const { return **(begin) - **(begin + 1); }
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const Counter<C>& c){	return c.Display(strm);}

#endif
