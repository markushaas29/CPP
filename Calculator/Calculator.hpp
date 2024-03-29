#include <memory>
#include <ratio>
#include "CalculatorResult.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

template<typename T, typename StageT>
class Calculator
{
public:
	using DependentQuantity = T::StageQuantity; 
	static void Calculate(DateTimes::Year y)
	{
		Logger::Log<Info>()<<"CALCULATION: \t"<<DependentQuantity::Key<<std::endl;
	};
};
	
	
	//~ template<class Config>
	//~ struct Stage
	//~ {
		//~ template<typename Stage, typename AllStages, typename ConfigT = Configuration<Config, Stage, AllStages>>
		//~ static std::shared_ptr<Result<ConfigT>> Calculate()
		//~ {
			//~ auto total = AllStages::Instance().template GetTotal<typename Config::QuantityType>();
			
			//~ auto raiba = Bank::Get<Bank::Raiba<0>, Bank::Out>();
			//~ auto account = raiba(Config::AccountKey);
			//~ account.Display(std::cout);
			
			//~ if(T::IsSame<Config,BuildingInsurance>::Value)
			//~ {
				//~ if(T::IsSame<Stage,Top>::Value)
					//~ Ratio::Calculate(IndividualUnit(2).Get(), IndividualUnit(4).Get(), Bank::GetTransferEndpoint<Quantity<Sum>>(account));
				//~ else
					//~ Ratio::Calculate(IndividualUnit(1).Get(), IndividualUnit(4).Get(), Bank::GetTransferEndpoint<Quantity<Sum>>(account));
				
				//~ return std::make_shared<Result<ConfigT>>();
			//~ }
			
			//~ auto result = Ratio::Calculate(GetStage<typename Config::QuantityType, typename Stage::Configuration>().Get(), total, Bank::GetTransferEndpoint<Quantity<Sum>>(account));
			
			//~ std::cout<<"CALC: "<<result<<std::endl;
			
			//~ return std::make_shared<Result<ConfigT>>();
		//~ }
	//~ };
	
	
	//~ template<>
	//~ struct Stage<PropertyTax>
	//~ {
		//~ template<typename Stage, typename AllStages, typename ConfigT = Configuration<PropertyTax, Stage, AllStages>>
		//~ static std::shared_ptr<Result<ConfigT>> Calculate()
		//~ {
			//~ auto totalQ = AllStages::Instance().template GetTotal<typename PropertyTax::QuantityType>();
			
			//~ auto raiba = Bank::Get<Bank::Raiba<0>, Bank::Out>();
			//~ auto account = raiba(PropertyTax::AccountKey);
			//~ account.Display(std::cout);
			//~ auto water = account.GetCause(PropertyTax::CauseString);
			//~ using QuantityType = Quantity<Sum>; 
			//~ auto totalSum = Quantity<Sum>(0); 
			
			//~ for(auto w : water)
				//~ totalSum = totalSum + Bank::GetTransfer<QuantityType>(*w);
			
			//~ Logger::Log()<<totalSum;
			
			//~ auto result = Ratio::Calculate(GetStage<typename PropertyTax::QuantityType, typename Stage::Configuration>().Get(), totalQ, totalSum);
			
			//~ std::cout<<"CALC: "<<result<<std::endl;
			
			//~ return std::make_shared<Result<ConfigT>>();
		//~ }
	//~ };
	
	//~ template<>
	//~ struct Stage<Sewage>
	//~ {
		//~ template<typename Stage, typename AllStages, typename ConfigT = Configuration<Sewage, Stage, AllStages>>
		//~ static std::shared_ptr<Result<ConfigT>> Calculate()
		//~ {
			//~ auto raiba = Bank::Get<Bank::Raiba<0>, Bank::Out>();
			//~ auto account = raiba(Sewage::AccountKey);
			//~ account.Display(std::cout);
			//~ auto sewage = account.GetCause(Sewage::CauseString);
			//~ using QuantityType = Quantity<Sum>; 
			//~ auto totalSum = Quantity<Sum>(0); 
			
			//~ for(auto s : sewage)
				//~ totalSum = totalSum + Bank::GetTransfer<QuantityType>(*s);
			
			//~ auto invoice = account.GetCause(Sewage::InvoiceString);
			//~ totalSum= totalSum + Bank::GetTransfer<QuantityType>(*(invoice.at(0)));
			
			//~ Logger::Log()<<"WATER sum"<<totalSum<<std::endl;
			//~ auto cwb = Stage::ColdWaterCounter::Instance().ConsumptionssBegin();
			//~ auto hwb = Stage::HotWaterCounter::Instance().ConsumptionssBegin();
			//~ auto water = CWA::Instance().ConsumptionssBegin();
			
			//~ for(int i = 0; cwb + i != Stage::ColdWaterCounter::Instance().ConsumptionsEnd(); ++i)
			//~ {
				//~ auto sum = (cwb + i)->Value + (hwb + i)->Value;
				//~ auto result = Ratio::Calculate(sum, (water+i)->Value, totalSum);
				//~ Logger::Log()<<"WATER"<<result<<std::endl;
				
			//~ }
			
			//~ Logger::Log(Stage::EnergyCounter::Instance().ConsumptionssBegin(), Stage::EnergyCounter::Instance().ConsumptionsEnd());
			//~ return std::make_shared<Result<ConfigT>>();
		//~ }
	//~ };
	
	
	//~ template<class TCounter, typename TCalc>
	//~ struct Counter
	//~ {
		//~ using ReadingType =  TCounter::ReadingType;
		//~ using QuantityType = TCounter::QuantityType;
		//~ static std::vector<ReadingResult<ReadingType,QuantityType>> Calculate()
		//~ {
			//~ auto result = std::vector<ReadingResult<ReadingType,QuantityType>>(); 
			//~ for(auto it = TCounter::Begin(); (it + 1) != TCounter::End(); ++it)
			//~ {
				//~ auto cr = TCalc::template Calculate<ReadingType,QuantityType>(*it, *(it+1));
				//~ result.push_back(cr);
			//~ }
			
			//~ return result;
		//~ }
	//~ };	
	
	template<class TCounter, typename TCalc>
	struct StageWater
	{
		using ReadingType =  TCounter::ReadingType;
		using QuantityType = TCounter::QuantityType;
		static decltype(auto) Calculate()
		{
			auto result = std::vector<Result<Addition,ReadingType, ReadingType,QuantityType>>(); 
			for(auto it = TCounter::Begin(); (it + 1) != TCounter::End(); ++it)
			{
				auto cr = TCalc::template Calculate<ReadingType,QuantityType>(*it, *(it+1));
				result.push_back(cr);
			}
			
			return result;
		}
	};	


#endif
