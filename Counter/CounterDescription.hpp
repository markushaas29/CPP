#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"

#pragma once

struct None 	{	inline static constexpr const char* Value = ""; };
struct Hot		{	inline static constexpr const char* Value = "Hot";	};
struct Cold		{	inline static constexpr const char* Value = "Cold"; };

template<typename T>
class Entity
{
public:
	using Unit = T;
};

struct EnergyEntity: public Entity<Energy> 	{	inline static constexpr const char* Name = "Energy"; };
struct WaterEntity: public Entity<Volume>	{	inline static constexpr const char* Name = "Water"; };
struct GasEntity: public Entity<Volume>		{	inline static constexpr const char* Name = "Gas"; };

template<typename E, typename S,  size_t No = 0,typename D = None, typename DM = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>
class CounterDescription: public DescriptorBase
{
public:
	using DataModel = DM;
	using Unit = E::Unit;
	CounterDescription(std::shared_ptr<Factory<IToken>> f, const std::string& p): tokenFactory{f}, path{createPath(p)} {}
	inline static constexpr uint Number = No;
	inline static constexpr const char* Entity = E::Name;
	inline static constexpr const char* Stage = S::Value;
	inline static std::string Identifier = std::string(Entity) + std::string(D::Value) + "_" + std::string(Stage) + "_" + std::to_string(No);
	static std::unique_ptr<CounterDescription> Make(std::shared_ptr<Factory<IToken>> f,const std::string& s) { return std::make_unique<CounterDescription>(f,s);	}
	virtual const std::string& Path() const { return path; };
	virtual std::unique_ptr<DataModel> Read() const 
	{
        auto mvr = MatrixReader(path);   
        auto mv = mvr.template M<2>();
        auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));
        return std::make_unique<DataModel>(mv.Parse(matcher));
    }
private:	
	std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	inline static constexpr const char* ending = ".csv";
	inline static std::string createPath(const std::string& p) { return p + "/" + Identifier + std::string(ending) ;};
	virtual std::ostream& display(std::ostream& out) const { 	out<<"Number\t"<<Number<<std::endl;
														out<<"Name\t"<<path<<std::endl;
				//										out<<"Unit\t"<<U::Sign()<<std::endl;
				//										out<<"SiUnit\t"<<U::SiUnit()<<std::endl;
														return out;	}
};


struct Top  	{	inline static constexpr const char* Value = "Top"; };
struct Bottom 	{	inline static constexpr const char* Value = "Bottom"; };
struct Middle  	{	inline static constexpr const char* Value = "Middle"; };
struct House  	{	inline static constexpr const char* Value = "House"; };

using GasDesc = CounterDescription<GasEntity,House,1202757>;
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
using TopHotWaterDesc = CounterDescription<WaterEntity,Top,25470737>;
