#include <map>
#include <functional>
#include <memory>
#include <string_view>
#include "../Is/Is.hpp"
#include "../String/Format.hpp"

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
	FactoryUnit(const IdentifierType& id, const ArgumentType& arg) : identifier{id}, argument{arg} {}
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

template<class T, typename CT = std::string>
class IFactory
{
public:
	inline static constexpr const char TypeIdentifier[] = "Factory";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	using Type = T;
	using PtrType = std::unique_ptr<T>;
	using ArgumentType = CT;
	using CreatorType = std::function<std::unique_ptr<T>(CT)>;
	using IdentifierType = std::string;
	virtual void Register(const IdentifierType& id, CreatorType c) = 0; 
	virtual const CreatorType& operator[](const IdentifierType& id) = 0;
	virtual PtrType operator()(const IdentifierType& id, const ArgumentType& arg)  = 0;
	virtual std::unique_ptr<std::vector<PtrType>> operator()(const std::vector<FactoryUnit<IdentifierType, ArgumentType>> units) = 0;
	virtual size_t Size() = 0;
protected:
	template<typename E> using IsT =  Is<E,TypeId>;
};

template<class T, typename CT = std::string>
class Factory: public IFactory<T,CT>
{
	using Base = IFactory<T,CT>; 
public:
	void Register(const typename Base::IdentifierType& id,  typename Base::CreatorType c) 
	{ 
		std::cout<<"REG->"<<id<<std::endl;
		creators.try_emplace(id,c); } 
	const typename Base::CreatorType& operator[](const  typename Base::IdentifierType& id) {	return find(id);	}
	typename Base::PtrType operator()(const typename Base::IdentifierType& id, const typename Base::ArgumentType& arg) { return find(id)(arg);}
	std::unique_ptr<std::vector< typename Base::PtrType>> operator()(const std::vector<FactoryUnit< typename Base::IdentifierType,  typename Base::ArgumentType>> units) 
	{
		auto result = std::make_unique<std::vector<typename Base::PtrType>>();
		for(auto u : units)
			result->push_back((*this)(u.Id(), u.Arg()));
		return result;
	}
	size_t Size() { return creators.size(); }
private:
	friend std::ostream& operator<<(std::ostream& out, const Factory& m) 
    { 
        std::for_each(m.creators.cbegin(), m.creators.cend(), [&](auto& t) { out<<t.first<<"\n"; });
        return out;
    }
	const typename Base::CreatorType& find(const typename Base::IdentifierType& id) 
	{
		auto i = creators.find(id);
		if(i == creators.end())
			typename Base::IsT<Throwing>(Format(id))(false);
		return (i->second); 
	}
	std::map< typename Base::IdentifierType, typename Base::CreatorType> creators;
};

template<class T, class F, typename CT = std::unique_ptr<std::vector<std::unique_ptr<T>>>>
class FactoryStack
{
public:
	using PtrType = std::unique_ptr<T>;
	using ElementType = typename T::ElementType;
	using IdentifierType = std::string;
	using ArgumentType = std::vector<FactoryUnit<IdentifierType, ElementType>>;
	using CreatorType = std::function<std::unique_ptr<T>(CT)>;
	using FactoryType = F; 
	inline static constexpr const char TypeIdentifier[] = "FactoryStack";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	FactoryStack(std::shared_ptr<FactoryType> f): factory{f} {}
	void Register(const IdentifierType& id,  CreatorType c) { creators.try_emplace(id,c); } 
	const CreatorType& operator[](const  IdentifierType& id) {	return find(id);	}
	PtrType operator()(const IdentifierType& id, const ArgumentType& arg) { return (*this)[id]((*factory)(arg));	}
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
	std::shared_ptr<FactoryType> factory;
	std::map< IdentifierType, CreatorType> creators;
};

template<class T, class F, typename CT = std::unique_ptr<IElement>>
class CompositeFactory
{
public:
	using PtrType = std::unique_ptr<T>;
	using IdentifierType = std::string;
	using ArgumentType = FactoryUnit<IdentifierType, IdentifierType>;
	using CreatorType = std::function<std::unique_ptr<T>(CT)>;
	using FactoryType = F; 
	inline static constexpr const char TypeIdentifier[] = "FactoryStack";
   	inline static constexpr Literal TypeId{TypeIdentifier};
	CompositeFactory(std::shared_ptr<FactoryType> f): factory{f} {}
	void Register(const IdentifierType& id,  CreatorType c) { creators.try_emplace(id,c); } 
	const CreatorType& operator[](const  IdentifierType& id) {	return find(id);	}
	PtrType operator()(const IdentifierType& id, const ArgumentType& arg) { return (*this)[id]((*factory)(arg.Id(), arg.Arg()));	}
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
	std::shared_ptr<FactoryType> factory;
	std::map< IdentifierType, CreatorType> creators;
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
