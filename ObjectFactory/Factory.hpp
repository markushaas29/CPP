 #include <map>
 #include <functional>
 #include <memory>

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

template<class T, class CreatorType= std::function<std::unique_ptr<T>()>, typename IdentifierType=std::string>
class Factory
{
public:
	using Type = T;
	bool Register(const IdentifierType& id, CreatorType c) { return creators.insert(id,c); } 
	std::unique_ptr<Type> operator()(const IdentifierType& id) 
	{
		auto i = creators.find(id);
		if(i != creators.end())
			return (i->second)(); 
		return nullptr;
	} 
private:
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
