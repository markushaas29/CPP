#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <memory>
#include "ParserState.hpp"
#include "ParsedValues.hpp"
#include "Creator.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.hpp"
#include "../Logger/Logger.hpp"

#ifndef ATOMS_HPP
#define ATOMS_HPP

namespace String_
{
	using It = std::string::const_iterator;

	template<typename>
	class Atom;
	
	template<typename T, typename ParsedT>
	class AtomBase
	{
	public:
		using State = ParserState;
		using StatePara = std::shared_ptr<ParserState>;
		using Base = AtomBase<T,ParsedT>;
		using ValueType = T;
		using ParsedValueType = ParsedT;
		using Type = Atom<T>;
		using ContainerType = std::vector<T>;
		static inline constexpr ParseKey Key = ParseKey(typeid(T).hash_code(), ParsedT::Key);
		
		static Type& Instance()
		{
			static Type instance;
			return instance;
		}
		
		bool make(StatePara state)
		{
			if(!Type::Instance().Is(state->Current()))
				return false;
			
			uint ctr = 0;
			while(Type::Instance().Is(state->Current() + ctr)) 
				++ctr;
				
			auto para = std::make_shared<std::string>(state->Current(),state->Current() + ctr);
			state->Add(Creator<ParsedT>::Parse(para, state->Counter(), state->Counter()+ctr, ctr));
			state->UpdateMap(ParseKey(typeid(T).hash_code(), ParsedT::Key), state->Counter());

			return true;
		}
		
		std::shared_ptr<ParserState> state;
	protected:
		inline static std::shared_ptr<ContainerType> values = std::make_shared<ContainerType>();

		~AtomBase()	{  }
		AtomBase& operator=(const AtomBase&){};
		AtomBase(const AtomBase& c){};
		AtomBase(){};
		Type& operator=(const Type&){};
	};
	
	template<typename T>
	class Atom: public AtomBase<T,ParsedWord>
	{
		friend class AtomBase<T,ParsedWord>;
	public:
		bool Is(It it){ return false; }
	};
	
	template<>
	class Atom<uint>: public AtomBase<uint, ParsedNumber>
	{
		friend class AtomBase<int,ParsedNumber>;
	public:
		bool Is(It it){ return std::isdigit(*it); }
	};
	
	template<>
	class Atom<std::string>: public AtomBase<std::string, ParsedWord>
	{
		friend class AtomBase<std::string,ParsedWord>;
	public:
		bool Is(It it){ return std::isalpha(*it); }
	};	
	
	template<>
	class Atom<ParsedPunct>: public AtomBase<ParsedPunct, ParsedPunct>
	{
		friend class AtomBase<ParsedPunct,ParsedPunct>;
	public:
		bool Is(It it){ return std::ispunct(*it); }
	};
		
	template<>
	class Atom<ParsedBlank>: public AtomBase<ParsedBlank, ParsedBlank>
	{
		friend class AtomBase<ParsedBlank, ParsedBlank>;
	public:
		bool Is(It it){ return std::isblank(*it); }
	};
		
	template<>
	class Atom<ParsedSpace>: public AtomBase<ParsedSpace, ParsedSpace>
	{
		friend class AtomBase<ParsedSpace, ParsedSpace>;
	public:
		bool Is(It it){ return std::isspace(*it); }
	};	
}

#endif
