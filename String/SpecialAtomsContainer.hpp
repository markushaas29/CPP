#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef SPECIALSpecialAtomContainer_HPP
#define SPECIALSpecialAtomContainer_HPP

//---------------------------------------------------------------------------------------------------SpecialAtomContainer----------------------------------------------------------------------------------------
namespace String_
{
	using ParaType = std::shared_ptr<std::string>;
	
	template<class B, typename List>
	class SpecialAtomContainer{};

	template<class B, typename Head>
	class SpecialAtomContainer<B, Typelist<Head>>
	{
	public:
		using Type = Head;
		using BaseType = Head;
		using SpecialAtomTypes = Typelist<Head>;
		using ContainerType = SpecialAtomContainer<B, Typelist<Head>>;
		using ReturnType = std::shared_ptr<B>;
	protected:
		SpecialAtomContainer() 
		{ 
			Logger::Log<Info>()<<Head::Value<<std::endl; 
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Type::Display(os);
		}
		
		ReturnType Parse(ParaType p)
		{
			Logger::Log()<<"IS: "<<(p->at(0)==Head::Value)<<" "<<Head::Value<<" "<<p->at(0)<<std::endl;
				return std::make_shared<B>(p, &Head::Is);
		}

		template<unsigned N>
		auto Get() { return At<SpecialAtomTypes,N>::Type; }

		static SpecialAtomContainer& Instance()
		{
			static SpecialAtomContainer instance;
			return instance;
		}	
	};

	template<class B, typename Head, typename... Tail>
	class SpecialAtomContainer<B, Typelist<Head,Tail...>>: public SpecialAtomContainer<B, Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using SpecialAtomTypes = Typelist<Head,Tail...>;
		using ContainerType = SpecialAtomContainer<B, Typelist<Head,Tail...>>;
		using Base = SpecialAtomContainer<B,Typelist<Tail...>>;
	protected:
		SpecialAtomContainer() { 
			Logger::Log<Info>()<<Head::Value<<std::endl; 
		};
	public:
		static std::ostream& Display(std::ostream& os) 
		{
			return Base::Display(Type::Display(os));		
		}
		
		template<unsigned N>
		auto Get() { return At<SpecialAtomTypes,N>::Type; }
		
		Base::ReturnType Parse(ParaType p)
		{
			if(p->at(0)==Head::Value)
			{
				Logger::Log()<<"IS1: "<<(p->at(0)==Head::Value)<<" "<<Head::Value<<" "<<p->at(0)<<std::endl;
				return std::make_shared<B>(p, &Head::Is);
			}
				
			return Base::Parse(p);
		}

		static SpecialAtomContainer& Instance()
		{
			static SpecialAtomContainer instance;
			return instance;
		}	
	};

	template<class B, typename Head, typename... Tail>
	std::ostream& operator<<(std::ostream& strm, const SpecialAtomContainer<B, Head,Tail...> c)
	{
		return c.Display(strm);
	}
}
#endif
