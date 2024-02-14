#include <map>
#include <functional>
#include <memory>
#include <string_view>
#include "../Is/Is.hpp"
#include "../String/Format.hpp"
#include "../Registration/Registration.hpp"

#pragma once

template<class ConcreteProduct>
class CreateFactoryUnitNewPolicy
{
public:
	static ConcreteProduct* DoCreate()
	{
		return new ConcreteProduct;
	}
};

template<typename IdentifierType=std::string, typename ArgumentType=std::string>
class FactoryUnit
{
public:
	FactoryUnit(const IdentifierType& id, const ArgumentType& arg = ArgumentType{}) : identifier{id}, argument{arg} {}
	decltype(auto) Arg() const { return argument; }
	decltype(auto) Add(const auto& fu) 
	{
		if constexpr (std::is_same_v<ArgumentType,std::vector<FactoryUnit<std::string, std::string>>>)
			argument.push_back(fu); 
	}
	decltype(auto) Id() const { return identifier; }
private:
	friend std::ostream& operator<<(std::ostream& s, const FactoryUnit& f) 
	{ 
		if constexpr (std::is_same_v<ArgumentType,std::vector<FactoryUnit<std::string, std::string>>>)
		{
			s<<f.identifier<<std::endl;
			std::for_each(f.argument.cbegin(), f.argument.cend(), [&](const auto& a) {s<<"\t"<<a<<"\n"; });
		}
		else
			s<<f.identifier<<": "<<f.argument;
		return s;
	}
	IdentifierType identifier;
	ArgumentType argument;
};

template<typename UnitType>
class FactoryUnitContainer
{
public:
	FactoryUnitContainer(const std::string& n, const UnitType& u): name{n}, units{u} {}
	decltype(auto) Units() const { return units; }
	decltype(auto) Name() const { return name; } 
private:
	friend std::ostream& operator<<(std::ostream& s, const FactoryUnitContainer& f) 
	{ 
		s<<f.name<<std::endl;
		std::for_each(f.units.cbegin(), f.units.cend(), [&](const auto& a) {s<<"\t"<<a<<"\n"; });
		return s;
	}
	std::string name;
	UnitType units;
};