#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef QUANTITYCONTAINER_HPP
#define QUANTITYCONTAINER_HPP

//---------------------------------------------------------------------------------------------------QuantityContainer----------------------------------------------------------------------------------------
namespace String_
{
	using ParaType = std::shared_ptr<std::string>;
	template<typename QuantityT>
	using QuantityMap = std::map<uint, QuantityT> ;
	
	template<typename T1, typename T2, typename Cont>
	struct QuantityGet
	{
		static auto apply()	{	return QuantityGet<T1,typename Cont::Base::Type, typename Cont::Base>::apply();	}
	};
	
	template<typename T, typename Cont>
	struct QuantityGet<T,T, Cont>
	{
		static auto apply()	{	return Cont::Instance().Quantities();	}
	};
	
	template<typename List>
	class QuantityContainer{};

	template<typename Head>
	class QuantityContainer<Typelist<Head>>
	{
	public:
		using Type = Head;
		using BaseType = Head;
		using SpecialAtomTypes = Typelist<Head>;
		using ContainerType = QuantityContainer<Typelist<Head>>;
		using MapType = std::shared_ptr<QuantityMap<Head>>;
	protected:
		QuantityContainer() { Logger::Log<Info>()<<index<<": "<<"Quantity "<<Head::UnitType::Name<<std::endl; };
		inline static uint index = 0;
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			for(auto it = quantities->cbegin(); it != quantities->cend(); ++it)
				os<<it->second<<std::endl;
			return os;
		}
		
		template<typename ValueType>
		const std::string Get(ValueType v,ParaType p){	return Head::UnitType::Name; }
		
		template<typename ValueType>
		void Add(ValueType v,ParaType p){ this->quantities->insert(std::pair<uint,Head>(0,Head(v) )); }
		
		const auto Quantities() { return quantities; }

		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
	private:
		static inline std::shared_ptr<QuantityMap<Head>> quantities = std::make_shared<QuantityMap<Head>>();
	};

	template<typename Head, typename... Tail>
	class QuantityContainer<Typelist<Head,Tail...>>: public QuantityContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using SpecialAtomTypes = Typelist<Head,Tail...>;
		using ContainerType = QuantityContainer<Typelist<Head,Tail...>>;
		using Base = QuantityContainer<Typelist<Tail...>>;
		using MapType = std::shared_ptr<QuantityMap<Head>>;
	protected:
		QuantityContainer() { Logger::Log<Info>()<<index<<": "<<"Quantity "<<Head::UnitType::Name<<std::endl; };
		inline static uint index = Base::index + 1; 
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			for(auto it = quantities->cbegin(); it != quantities->cend(); ++it)
				os<<it->second<<std::endl;
			return Base::Display(os);		
		}
		
		template<typename ValueType>
		const std::string Get(ValueType v,ParaType p)
		{
			  if(*p==Head::UnitType::Sign())
					return Head::UnitType::Name;

              return Base::Instance().Get(v,p);
		}
		
		template<typename T>
		const auto Get(){	return QuantityGet<T,Type, ContainerType>::apply();	};
				
		template<typename Q>
		size_t Size(){ return QuantityGet<Q,Type, ContainerType>::apply()->size();	}
		
		const auto Quantities() { return quantities; }
		
		template<typename ValueType>
		void Add(ValueType v,ParaType p)
		{
			if(*p==Head::UnitType::Sign())
			{	
				this->quantities->insert(std::pair<uint,Head>(0,Head(v) ));
				Logger::Log()<<"Add "<<this->quantities<<" "<<Type::UnitType::Name<<" "<<this->quantities->size()<<std::endl;
				return;	
			}
			Base::Add(v,p);
		}

		static QuantityContainer& Instance()
		{
			static QuantityContainer instance;
			return instance;
		}	
		
	private:
		static inline std::shared_ptr<QuantityMap<Head>> quantities = std::make_shared<QuantityMap<Head>>();
	};

	template<typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const QuantityContainer<Head,Tail...> c){	return c.Display(strm);	}
	
}
#endif
