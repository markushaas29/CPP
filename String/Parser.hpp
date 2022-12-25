#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "AtomContainer.hpp"
#include "Atoms.hpp"
#include "ParserState.hpp"
#include "ParsedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ATOMPASSINGPARSER_HPP
#define ATOMPASSINGPARSER_HPP

namespace String_
{
	class Parser
	{	
		template<typename T>
		using ReturnType = std::shared_ptr<std::vector<std::shared_ptr<T>>>;
	public:
		using Types = Typelist<Atom<ParsedPunct>, Atom<ParsedBlank>, Atom<ParsedSpace>,Atom<uint>,Atom<std::string>,Atom<char>>;
		using AtomContainerType = AtomContainer<Types>::ContainerType;
		using ValueType = std::shared_ptr<ParsedValue>;
		
		Parser(): state(std::make_shared<ParserState>()){	};
				
		void Parse(std::shared_ptr<std::string> sp)
		{
			this->state->Set(sp);
			while(this->state->Current()!= this->state->End())
				AtomContainerType::Instance().make(this->state);
			
			this->state->Display(std::cout);
		}
		
		void Parse(const std::string& s)
		{
			this->Parse(std::make_shared<std::string>(s));
		}
		
		
		template<typename T>
		ReturnType<T> Get()
		{
			auto ret = std::make_shared<std::vector<std::shared_ptr<T>>>();
			
			for(auto it = this->state->Values()->begin(); it != this->state->Values()->end(); ++it)
				if(auto cv = std::dynamic_pointer_cast<T>(*it))
					ret->push_back(cv);
					
			return ret;
		}		
		
		ParserState::ContainerParaType Values(){ return this->state->Values(); };
		
		virtual ~Parser(){};
	private:		
		std::shared_ptr<ParserState> state;
	};
	
}

#endif
