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

template<typename P>
concept UnitConcept = requires(P p)
{
    p.Arg();
    p.Id();
};

template<typename IdentifierType=std::string, typename ArgumentType=std::string>
class FactoryUnit
{
public:
	FactoryUnit() = default;
	FactoryUnit(const IdentifierType& id, const ArgumentType& arg= ArgumentType{}) : identifier{id}, argument{arg} {}
	decltype(auto) Arg() const { return argument; }
	decltype(auto) Add(const auto& fu) 
	{
		if constexpr (std::is_same_v<ArgumentType,std::vector<FactoryUnit<std::string, std::string>>>)
			argument.push_back(fu); 
	}
	decltype(auto) Id() const { return identifier; }
private:
	friend std::istream& operator>>(std::istream& s, FactoryUnit& f) 
	{ 
        std::string b;
		IdentifierType id;
		ArgumentType arg;

		s>>b;
		if(b!="{")
			return s;
		s>>id;
		s>>arg;
		f = FactoryUnit(id,arg);
		s>>b;

		return s;
	}
	friend std::ostream& operator<<(std::ostream& s, const FactoryUnit& f) 	{	return	s<<"{ "<<f.identifier<<" "<<f.argument<<" }";	}
	IdentifierType identifier;
	ArgumentType argument;
};

template<typename T, typename A>
FactoryUnit(const T&, const A&) -> FactoryUnit<T,A>;

template<typename UnitT>
class IFactoryUnitContainer
{
public:
	using UnitType = UnitT;
	decltype(auto) Units() const { return values(); }
	decltype(auto) Name() const { return iname(); } 
	decltype(auto) Size() const { return num(); } 
private:
//	friend std::ostream& operator<<(std::ostream& s, const FactoryUnitContainer& f) 
//	{ 
//		s<<f.name<<std::endl;
//		std::for_each(f.units.cbegin(), f.units.cend(), [&](const auto& a) {s<<"\t"<<a<<"\n"; });
//		return s;
//	}
	virtual std::string iname() const = 0;
	virtual std::vector<UnitType> values() const = 0;
	virtual size_t num() const = 0;
};

template<typename UnitType>
class FactoryUnitContainer: public IFactoryUnitContainer<UnitType>
{
public:
	FactoryUnitContainer(const std::string& n, const std::vector<UnitType>& u): name{n}, units{u} {}
private:
	friend std::ostream& operator<<(std::ostream& s, const FactoryUnitContainer& f) 
	{ 
		s<<"["<<f.name<<" [ "<<"\n";
		std::for_each(f.units.cbegin(), f.units.cend(), [&](const auto& a) {s<<"\t"<<a<<"\n"; });
		s<<" ] ]";
		return s;
	}
	friend std::istream& operator>>(std::istream& s, FactoryUnitContainer& f) 
	{ 
        std::string b, str, id, arg;
		std::vector<UnitType> units;
		s>>b;
		if(b!="[")
			return s;
		s>>id;
			std::cout<<"Id "<<id<<std::endl;
		while(s)
		{
        	s>>str;
			std::cout<<"Unit "<<str<<std::endl;
			if(str=="{")
			{
				s.putback(str[0]);
				UnitType u;
				s>>u;
			std::cout<<"Unit "<<u<<std::endl;
			units.push_back(u);
			}
		}
		f.name = id;
		f.units = units;
		return s;
	}
	virtual std::string iname() const { return name; };
	virtual std::vector<UnitType> values() const { return units; };
	virtual size_t num() const { return 1; };
	std::string name;
	std::vector<UnitType> units;
};


template<typename T>
FactoryUnitContainer(const std::string&, const std::vector<T>&) -> FactoryUnitContainer<T>;
