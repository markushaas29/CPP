#include "../Unit/Unit.hpp"

#pragma once

class None {	inline static constexpr const char* Value = ""; };
class Hot	{	inline static constexpr const char* Value = "Hot";	};
class Cold	{	inline static constexpr const char* Value = "Cold"; };

class Top  {	inline static constexpr const char* Value = ""; };
class Bottom {	inline static constexpr const char* Value = ""; };
class Middle  {	inline static constexpr const char* Value = ""; };
class All  {	inline static constexpr const char* Value = ""; };

template<typename T>
class Entity
{
public:
	using Unit = T;
};

class EnergyEntity: public Entity<Energy>
{	
	inline static constexpr const char* Id = "Energy"; 
};

class WaterEntity: public Entity<Volume>
{	
	inline static constexpr const char* Id = "Water"; 
};

class GasEntity: public Entity<Volume>
{	
	inline static constexpr const char* Id = "Gas"; 
};


class IDescription
{
	virtual std::ostream& display(std::ostream& out) const = 0;
	friend std::ostream& operator<<(std::ostream& out, const IDescription& i) {	return i.display(out); } 
};

template<typename E, typename S,  size_t No = 0>
class CounterDescription: public IDescription
{
public:
	inline static constexpr uint Number = No;
	inline static std::string CounterName = std::to_string(No) + "_" ;//+ std::string(MeterType::Name) + std::string(AdditionalInformation);
	using Unit = E::Unit;
private:	
	virtual std::ostream& display(std::ostream& out) const { 	out<<"Number\t"<<Number<<std::endl;
				//										out<<"Name\t"<<CounterName<<std::endl;
				//										out<<"Type\t"<<MeterType::Name<<std::endl;
				//										out<<"Unit\t"<<U::Sign()<<std::endl;
				//										out<<"SiUnit\t"<<U::SiUnit()<<std::endl;
														return out;	}
};


using GasDescription = CounterDescription<GasEntity,Top,1202757>;
//using EnBWHouseEnergyDescription = CounterDescription<Energy,21740069, Work>;
//using EnBWHallEnergyDescription = CounterDescription<Energy,11256983, Work>;
//using VattenfallEnergyDescription = CounterDescription<Energy,11144078, Work>;
//using AllWaterDescription = CounterDescription<Water,18061860, Volume>;
//using OWaterDescription = CounterDescription<Water,279638, Volume>;
//using OutWaterDescription = CounterDescription<Water,458711, Volume>;
//using Bottom_HWaterDescription = CounterDescription<Water,15018324, Volume, AdditionalInformation<Hot>>;
//using Bottom_CWaterDescription = CounterDescription<Water,15007241, Volume, AdditionalInformation<Cold>>;
//using Middle_HWaterDescription = CounterDescription<Water,23267492, Volume, AdditionalInformation<Hot>>;
//using Middle_CWaterDescription = CounterDescription<Water,14524889, Volume, AdditionalInformation<Cold>>;
//using Top_CWaterDescription = CounterDescription<Water,25489823, Volume, AdditionalInformation<Cold>>;
//using Top_HWaterDescription = CounterDescription<Water,25470737, Volume, AdditionalInformation<Hot>>;
