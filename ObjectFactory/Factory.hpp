#include <map>
#include <functional>
#include <memory>
#include <string_view>
#include "../Is/Is.hpp"
#include "../String/Format.hpp"

#pragma once

//template<class T>
//class FactoryUnit
//{
//public:
//	using Type = T;
//	virtual T* DoCreate() = 0;
//	virtual void Test() = 0;
//	virtual ~FactoryUnit(){} 
//};

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
	FactoryUnit(const IdentifierType& id, const ArgumentType& arg) : identifier{id}, argument{arg} {}
	decltype(auto) Arg() { return argument; }
	decltype(auto) Id() { return identifier; }
private:
	const IdentifierType identifier;
	const ArgumentType argument;
};

template<class T, typename CT = std::string, class CreatorType= std::function<std::unique_ptr<T>(const CT&)>, typename IdentifierType=std::string>
class IFactory
{
public:
	inline static constexpr const char TypeIdentifier[] = "Factory";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	using Type = T;
	using PtrType = std::unique_ptr<T>;
	using ArgumentType = CT;
	virtual void Register(const IdentifierType& id, CreatorType c) = 0; 
	virtual const CreatorType& operator[](const IdentifierType& id) = 0;
	virtual PtrType operator()(const IdentifierType& id, const ArgumentType& arg)  = 0;
	virtual std::unique_ptr<std::vector<PtrType>> operator()(const std::vector<FactoryUnit<IdentifierType, ArgumentType>> units) = 0;
	virtual size_t Size() = 0;
private:
	template<typename E> using IsT =  Is<E,TypeId>;
};

template<class T, typename CT = std::string, class CreatorType= std::function<std::unique_ptr<T>(const CT&)>, typename IdentifierType=std::string>
class Factory: IFactory<T,CT,CreatorType,IdentifierType>
{
public:
	inline static constexpr const char TypeIdentifier[] = "Factory";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	using Type = T;
	using PtrType = std::unique_ptr<T>;
	using ArgumentType = CT;
	void Register(const IdentifierType& id, CreatorType c) { creators.try_emplace(id,c); } 
	const CreatorType& operator[](const IdentifierType& id) {	return find(id);	}
	PtrType operator()(const IdentifierType& id, const ArgumentType& arg) { return find(id)(arg);}
	std::unique_ptr<std::vector<PtrType>> operator()(const std::vector<FactoryUnit<IdentifierType, ArgumentType>> units) 
	{
		auto result = std::make_unique<std::vector<PtrType>>();
		for(auto u : units)
			result->push_back((*this)(u.Id(), u.Arg()));
		return result;
	}
	size_t Size() { return creators.size(); }
private:
	template<typename E> using IsT =  Is<E,TypeId>;
	const CreatorType& find(const IdentifierType& id) 
	{
		auto i = creators.find(id);
		if(i == creators.end())
			IsT<Throwing>(Format(id))(false);
		return (i->second); 
	}
	std::map<IdentifierType,CreatorType> creators;
};

template<class TList, template<class> class Unit = FactoryUnit, template<class> class CreatePolicy = CreateFactoryUnitNewPolicy>
class AbstractFactory
{
public:
	using ProductList = TList;
	
	template<class T> 
	T* Create()
	{
		Unit<T>* unit = CreatePolicy<T>::DoCreate();
		return unit->DoCreate();
	}
};
