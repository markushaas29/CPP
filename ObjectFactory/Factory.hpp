#include <map>
#include <functional>
#include <memory>
#include <string_view>
#include "../Is/Is.hpp"
#include "../String/Format.hpp"

#pragma once

template<class T>
class FactoryUnit
{
public:
	using Type = T;
	virtual T* DoCreate() = 0;
	virtual void Test() = 0;
	virtual ~FactoryUnit(){} 
};

template<class ConcreteProduct>
class CreateFactoryUnitNewPolicy
{
public:
	static ConcreteProduct* DoCreate()
	{
		return new ConcreteProduct;
	}
};

template<class T, class CreatorType= std::function<std::unique_ptr<T>(const std::string&)>, typename IdentifierType=std::string>
class Factory
{
public:
	inline static constexpr const char TypeIdentifier[] = "Equivalence";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	using Type = T;
	void Register(const IdentifierType& id, CreatorType c) { creators.try_emplace(id,c); } 
	const CreatorType& operator[](const IdentifierType& id) {	return find(id);	}
	std::unique_ptr<Type> operator()(const IdentifierType& id, const std::string& arg) //{ return fin(id)();}
	{
		auto i = creators.find(id);
		if(i != creators.end())
			return (i->second)(arg); 
		IsT<Throwing>(Format(id));
	}
	decltype(auto) Size() { return creators.size(); }
private:
	template<typename E> using IsT =  Is<E,TypeId>;
	const CreatorType& find(const IdentifierType& id) 
	{
		auto i = creators.find(id);
		if(i == creators.end())
			IsT<Throwing>(Format(id));
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
