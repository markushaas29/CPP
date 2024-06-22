#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"

#pragma once

struct None 	{	inline static constexpr const char* Name = ""; };
struct Hot		{	inline static constexpr const char* Name = "Hot";	};
struct Cold		{	inline static constexpr const char* Name = "Cold"; };

template<typename T>
struct Entity
{
	using Unit = T;
};

struct EnergyEntity: public Entity<Energy> 	{	inline static constexpr const char* Name = "Energy"; };
struct WaterEntity: public Entity<Volume>	{	inline static constexpr const char* Name = "Water"; };
struct GasEntity: public Entity<Volume>		{	inline static constexpr const char* Name = "Gas"; };


template<size_t I>
struct Stage
{
	static inline constexpr int Index = I;
};

struct Top: public Stage<3>  	{	inline static constexpr const char* Name = "Top"; };
struct Bottom: public Stage<1> 	{	inline static constexpr const char* Name = "Bottom"; };
struct Middle: public Stage<2>  	{	inline static constexpr const char* Name = "Middle"; };
struct House: public Stage<0>  	{	inline static constexpr const char* Name = "House"; };

template<typename E, typename S,  size_t No = 0,typename D = None, typename DM = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>
class CounterDescription: public DescriptorBase
{
public:
	using DataModel = DM;
	using Unit = E::Unit;
	CounterDescription(std::shared_ptr<Factory<IToken>> f, const std::string& p): tokenFactory{f}, path{p},filename{createPath(p)} {}
	inline static constexpr uint Number = No;
	inline static constexpr const char* Entity = E::Name;
	inline static constexpr const char* Stage = S::Name;
	inline static std::string Identifier = std::string(Entity) + std::string(D::Name) + "_" + std::string(Stage) + "_" + std::to_string(No);
	static std::unique_ptr<CounterDescription> Make(std::shared_ptr<Factory<IToken>> f,const std::string& s) { return std::make_unique<CounterDescription>(f,s);	}
	virtual const std::string& Path() const { return path; };
	virtual const std::string& Filename() const { return filename; };
private:	
	std::string path;
	std::string filename;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	inline static constexpr const char* ending = ".csv";
	inline static std::string createPath(const std::string& p) { return p + "/" + Identifier + std::string(ending) ;};
	virtual std::ostream& display(std::ostream& out) const { 	out<<"Number\t"<<Number<<std::endl;
														out<<"Name\t"<<path<<std::endl;
				//										out<<"Unit\t"<<U::Sign()<<std::endl;
				//										out<<"SiUnit\t"<<U::SiUnit()<<std::endl;
														return out;	}
};


using GasDesc = CounterDescription<GasEntity,House,1202757>;
using EnBWHouseEnergyDesc = CounterDescription<EnergyEntity,House,21740069>;
using EnBWHallEnergyDesc = CounterDescription<EnergyEntity,House,11256983>;
using VattenfallEnergyDesc = CounterDescription<EnergyEntity,House,11144078>;
using AllWaterDesc = CounterDescription<WaterEntity,House,18061860>;
//using OWaterDesc = CounterDescription<WaterEntity,279638>;
//using OutWaterDesc = CounterDescription<WaterEntity,458711>;
using BottomHotDesc = CounterDescription<WaterEntity,Bottom,15018324>;
using BottomColdDesc = CounterDescription<WaterEntity,Bottom,15007241>;
using MiddleHotDesc = CounterDescription<WaterEntity,Middle,23267492>;
using MiddleColdDesc = CounterDescription<WaterEntity,Middle,14524889>;
using TopColdDesc = CounterDescription<WaterEntity,Top,25489823>;
using TopHotDesc = CounterDescription<WaterEntity,Top,25470737>;
