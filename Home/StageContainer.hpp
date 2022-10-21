#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "Stage.hpp"
//~ #include "UtilitiesStatement.hpp"
#include "../Logger/Logger.hpp"
#include "../File/Node.hpp"
#include "../Calculator/Calculator.hpp"
#include "../Typelist/Typelist.h"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../Common/DateTimes.hpp"

#ifndef STAGECONTAINER_HPP
#define STAGECONTAINER_HPP

//---------------------------------------------------------------------------------------------------StageContainer----------------------------------------------------------------------------------------

using StagesMap = std::map<std::string, StageMap>;

template<typename Head>
class StageContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using StageTypes = Typelist<Head>;
	using ContainerType = StageContainer<Typelist<Head>>;
private:
	inline static constexpr const char* DestinationPath = "//home//markus//Downloads//CSV_Files//";
	inline static constexpr const char* Name = "Stages";
protected:
	inline static std::unique_ptr<StagesMap, DebugDeleter<StagesMap>> stages = std::unique_ptr<StagesMap, DebugDeleter<StagesMap>>(new StagesMap(),DebugDeleter<StagesMap>());
	inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(std::string(DestinationPath) + std::string(Name))));
	inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
	using InputIterator = std::vector<std::string>::const_iterator;
	using CsvValuesIterator = std::vector<std::vector<std::string>>::const_iterator;
	
	StageContainer() 
	{ 
 		Read();
		Head::Set(stages->at(Head::StageName)); 
		Head::Instance(); 			
		Logger::Log<Info>()<<"StageContainer created."<<Head::StageName<<std::endl; 
	};
	
	template<typename T, typename Op>
	decltype(auto) GetCounterTotalInternal(const Op&& op)
	{ 
		if constexpr (std::is_same<T,Hot>::value)
			return op(Head::HotWaterCounter::Instance().ReadingsBegin(), Head::HotWaterCounter::Instance().ReadingsEnd());
		return op(Head::ColdWaterCounter::Instance().ReadingsBegin(), Head::HotWaterCounter::Instance().ReadingsEnd());	
	}
public:
	static std::ostream& Display(std::ostream& os) 	{	return Type::Instance().Display(os); }	
	void Write(const std::string sourcePath = ".")	{ Type::Write(sourcePath); }
	static constexpr const char* GetName(){ return Name; }	
	static constexpr const char* GetFileName(){ return (std::string(Name) + ".csv").c_str(); }
	
	static void Parse(InputIterator begin, InputIterator end)
	{
		std::vector<std::vector<std::string>> csvValues;
		for(auto it = begin; it != end; ++it)
			csvValues.push_back(FS::CSV::ExtractValues(*it));
		
		ExtractValues(csvValues.cbegin(),csvValues.cend());
	}
	
	static void Get(const std::string& s) { Logger::Log("GET in",Name); }
	static bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in",Name); return true; }
	
	template<typename Cont>
	static void RegisterTo(Cont& cont){ cont.insert(std::make_pair(GetFileName(),  typename Cont::mapped_type(GetFileName(), &Parse, &Get, &Update))); }
	
	template<typename T>
	static decltype(auto) GetTotal() {	return GetStage<Head,T>().GetQuantity(); }

	static StageContainer& Instance()
	{
		static StageContainer instance;
		return instance;
	}
	
	template<typename AllT, template<typename> class CalcT>
	void CalculateInternal()
	{
		struct Config
		{
			using Stage = Head;
			using All = AllT;
		};
		CalcT<Type>::Instance();
	}
private:
	static void ExtractValues(CsvValuesIterator begin, CsvValuesIterator end)
	{
		auto keysIt = begin->cbegin();
		for(auto it = ++begin; it != end; ++it)
		{
			auto stage = StageMap();
			int j = 1;
			for(auto jt = ++(it->cbegin()); jt != it->cend(); ++jt, ++j)
				stage.insert(std::pair<std::string,std::string>(*(keysIt+j), *jt));
			
			stages->insert(std::pair<std::string,StageMap>(*(it->cbegin()), stage));
		}		
	}
	
	void Read(const std::string sourcePath = ".")
	{
		auto lines =  FS::ReadLines(DestinationPath + std::string(Name) +".csv");
		std::vector<std::vector<std::string>> values;
		for(auto line : lines)
			values.push_back(FS::CSV::ExtractValues(line));
		ExtractValues(values.cbegin(),values.cend());
	}
	
};

template<typename Head, typename... Tail>
class StageContainer<Typelist<Head,Tail...>>: public StageContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using StageTypes = Typelist<Head,Tail...>;
	using ContainerType = StageContainer<Typelist<Head,Tail...>>;
	using Base = StageContainer<Typelist<Tail...>>;
protected:
	StageContainer() 
	{ 
		Logger::Log<Info>()<<"StageContainer created."<<Head::StageName<<std::endl; 
		Type::Set(Base::stages->at(Head::StageName)); 			
		Type::Instance(); 			
	};
	
	template<typename T, typename Op>
	decltype(auto) GetCounterTotalInternal(const Op&& op)
	{ 
		if constexpr (std::is_same<T,Hot>::value)
			return op(Head::HotWaterCounter::Instance().ReadingsBegin(), Head::HotWaterCounter::Instance().ReadingsEnd()) + Base::template GetCounterTotalInternal<T>(std::move(op));
		return op(Head::ColdWaterCounter::Instance().ReadingsBegin(), Head::HotWaterCounter::Instance().ReadingsEnd()) + Base::template GetCounterTotalInternal<T>(std::move(op));	
	}
public:
	static std::ostream& Display(std::ostream& os) 	{	return Base::Display(Type::Instance().Display(os));	}

	static StageContainer& Instance()
	{
		static StageContainer instance;
		return instance;
	}	
	
	template<typename AllT, template<typename> class CalcT>
	void CalculateInternal()
	{
		struct Config
		{
			using Stage = Head;
			using All = AllT;
		};
		CalcT<Type>::Instance();
		Base::template CalculateInternal<AllT,CalcT>();
	}
	
	template<template<typename> class CalcT>
	void Calculate(){	CalculateInternal<ContainerType,CalcT>();	}
		
	template<typename T, typename Op>
	decltype(auto) GetCounterTotal(){	return GetCounterTotalInternal<T>(std::move(Op()));	}
	
	template<typename T>
	decltype(auto) GetTotal(){	return GetStage<Head,T>().GetQuantity() + Base::template GetTotal<T>();		}
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const StageContainer<Head,Tail...> c){	return c.Display(strm);}

using StageContainerType = StageContainer<Typelist<Top,Middle,Bottom>>::ContainerType;

#endif
