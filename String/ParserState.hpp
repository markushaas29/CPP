#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "ParsedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Logger/Logger.hpp"

#ifndef PARSERSTATE_HPP
#define PARSERSTATE_HPP

namespace String_
{
	struct ParseKey
	{
		const size_t Hash;
		const char* Key;
		constexpr ParseKey(size_t h, const char* k): Hash{h}, Key{k} { }
		bool operator<(const ParseKey& k) const{ return Key < k.Key;}
	};
	
	struct ParserState
	{
		using Iterator = std::string::const_iterator;
		using ValueType = std::shared_ptr<ParsedValue>;
		using MapType = std::map<ParseKey, std::shared_ptr<std::vector<uint>>>;
		using ContainerType = std::vector<ValueType>;
		using ContainerParaType = std::shared_ptr<ContainerType>;
		
		Iterator Current() { return (this->value->cbegin() + ctr);}
		uint CurrentPos() { return this->ctr;}
		Iterator End() { return this->value->cend();}
		Iterator Begin() { return this->value->cbegin();}
		
		char CurrentVal() { return *(this->value->cbegin() + ctr);}
		void Increment() { ++ctr; }
		uint Counter(){ return this->ctr; }
		
		void Set(std::shared_ptr<std::string> s)
		{
			this->reset();
			this->value = s;
		}
		std::shared_ptr<std::string> Get() const {  return this->value;}
		ContainerParaType Values(){ return this->parsedValues; }
		
		void Add(ContainerParaType v)
		{ 
			std::for_each(v->cbegin(), v->cend(), [this](auto i){ this->Add(i); });
		}
		
		void Add(ValueType v)
		{ 
			if(this->parsedValues->size() > 0 && (*(this->parsedValues->end()-1))->Next() == nullptr)
				(*(this->parsedValues->end()-1))->setNext(v);
			
			this->ctr += v->Value()->size();	
			v->setCounter(this->parsedValues->size());
			this->parsedValues->push_back(v); 
		}
		
		void UpdateMap(ParseKey k, uint begin) 
		{
			if ( !this->valuesMap->contains(k) )  
				this->valuesMap->insert({k,std::make_shared<std::vector<uint>>()} );
			
			this->valuesMap->find(k)->second->push_back(begin);
		}
		
		std::ostream& Display(std::ostream& out)
        {
			for(auto it = this->parsedValues->cbegin(); it != this->parsedValues->cend(); ++it)
				(*it)->Display(out);
					
			for(auto it = this->valuesMap->cbegin(); it != this->valuesMap->cend(); ++it)
			{
				out<<"\n"<<it->first.Key<<": ";
				for(auto i = it->second->cbegin(); i != it->second->cend(); ++i)
					out<<*i<<",";				
			}
					
			out<<std::endl;
			
			return out;
        }
        
	private:
		uint ctr = 0;
		std::shared_ptr<ContainerType> parsedValues = std::make_shared<ContainerType>();
		std::shared_ptr<MapType> valuesMap = std::make_shared<MapType>();
		std::shared_ptr<std::string> value = std::make_shared<std::string>();
        
        void reset()
        { 
			this->parsedValues->clear(); 
			this->valuesMap->clear();
			this->value = nullptr;
			this->ctr = 0;
		}
	};
	
	std::ostream& operator<<(std::ostream& strm, std::shared_ptr<ParserState> c)
	{
	        return c->Display(strm);
	}

}

#endif
