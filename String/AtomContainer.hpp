#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "Atoms.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef ATOMCONTAINER_HPP
#define ATOMCONTAINER_HPP

//---------------------------------------------------------------------------------------------------AtomContainer----------------------------------------------------------------------------------------

template<typename List>
class AtomContainer{};

template<typename Head>
class AtomContainer<Typelist<Head>>
{
public:
	using Type = Head;
	using AtomTypes = Typelist<Head>;
	using ContainerType = AtomContainer<Typelist<Head>>;
protected:
	AtomContainer() 
	{ 
		Head::Instance();
		Logger::Log<Info>()<<"AtomContainer created."<<std::endl; 
	};
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Type::Display(os);
	}
	
	template<typename ParserState>
	bool make(std::shared_ptr<ParserState> state)
	{
		return Head::Instance().make(state);
	}

	template<unsigned N>
	auto Get() { return At<AtomTypes,N>::Type; }

	static AtomContainer& Instance()
	{
		static AtomContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
class AtomContainer<Typelist<Head,Tail...>>: public AtomContainer<Typelist<Tail...>>
{
public:
	using Type = Head;
	using AtomTypes = Typelist<Head,Tail...>;
	using ContainerType = AtomContainer<Typelist<Head,Tail...>>;
	using Base = AtomContainer<Typelist<Tail...>>;
protected:
	AtomContainer() { 
		Head::Instance();
		Logger::Log<Info>()<<"AtomContainer created."<<std::endl; 
	};
public:
	static std::ostream& Display(std::ostream& os) 
	{
		return Base::Display(Type::Display(os));		
	}
	
	template<unsigned N>
	auto Get() { return At<AtomTypes,N>::Type; }
	
	template<typename ParserState>
	bool make(std::shared_ptr<ParserState> state)
	{
		Head::Instance().make(state);
		return Base::make(state);
	}

	static AtomContainer& Instance()
	{
		static AtomContainer instance;
		return instance;
	}	
};

template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& strm, const AtomContainer<Head,Tail...> c)
{
	return c.Display(strm);
}

#endif
