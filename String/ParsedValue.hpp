#include <string>
#include <cstring>
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <limits>
#include "SpecialAtomsContainer.hpp"
#include "../Common/Number.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef PARSEDVALUE_HPP
#define PARSEDVALUE_HPP

namespace String_
{
	struct ParserState;
	
	template<typename T>
	using ReturnT = std::shared_ptr<std::vector<std::shared_ptr<T>>>;
        
    class ParsedValue;
	using BasePtrType = std::shared_ptr<ParsedValue>;
    
    template<typename TVal, typename TParsed> struct IsImpl{ bool operator ()(TParsed bp){ Logger::Log()<<"T"<<std::endl; return false;}};
    template<typename TVal, typename TParsed> struct CastImpl{ bool operator ()(TParsed bp){ Logger::Log()<<"T"<<std::endl; return false;}};
	
	class ParsedValue
	{
		friend class ParserState;
	public:
		using ParaType = std::shared_ptr<std::string>;
		using BasePtrType = std::shared_ptr<ParsedValue>;
		using ContainerType = std::vector<BasePtrType>;
		using ContainerParaType = std::shared_ptr<ContainerType>;
		
		BasePtrType Next() const { return this->next; };
		ParaType Value() const { return this->strValue;}
		
		virtual ContainerParaType Get(ParaType p) { return ContainerParaType(); } 
		
		std::ostream& Display(std::ostream& out) const
        {
			out<<this->ctr<<": "<<*(this->strValue)<<std::endl;			
			return out;
        }
        
        template<typename TVal>
        bool Is()
        {
			auto is = std::make_shared<IsImpl<TVal,BasePtrType>>();
			return (*is)(std::make_shared<ParsedValue>(*this));
		}
		
        template<typename TVal>
        TVal Cast()
        {
			constexpr auto max = std::numeric_limits<TVal>::max();
			uint digitsMax = Number::Digits<max>::Value;

			if(digitsMax < this->Size())
				throw "Cast Error";
				
			std::ostringstream os;
			os << max;
			std::string digits = os.str();
			
			if(digitsMax == this->Size())
				for(auto i = digitsMax; i >= 0; --i)
					if(this->strValue->at(i) > digits.at(i))
						throw "Cast Error";
			
			TVal result = 0;
			uint e = this->Size()-1;
			for(uint i = 0; i < this->Size(); ++i)
				result += (TVal)(std::pow(10,e-i) * (this->strValue->at(i) - '0'));
				
			//~ Logger::Log()<<"MAX D: "<<digits<<" "<<digits[0]<<"_"<<digits.size()<<" RES: "<<result<<std::endl;
			return result;
		}
        
        char operator[](uint i) const{ return this->strValue->at(i); }
        size_t Size() const { return this->strValue->size(); }
        uint Counter() const { return this->ctr; }
        uint Start() const { return this->startIndex; }
        uint End() const { return this->endIndex; }
        
		ParsedValue(const ParsedValue& c)
		{
			this->next = c.Next();
			this->strValue = c.Value();
			this->ctr = c.Counter();
			this->startIndex = c.Start();
			this->endIndex = c.End();
		};
		virtual ~ParsedValue()	{ }
	protected:
		uint ctr;
		uint startIndex;
		uint endIndex;
		ParaType strValue;
		BasePtrType next;

		ParsedValue& operator=(const ParsedValue&){ return *this;};
		ParsedValue(BasePtrType n = nullptr): next{n}{};
		ParsedValue(ParaType s, uint start = 0, uint end = 0, uint c = 0, BasePtrType n = nullptr): next{n}, strValue{s}, startIndex{start},endIndex{end},ctr{c}{ };
        
        static auto extractCommonAtoms(ParsedValue::ParaType val)
        {
			auto ret = std::make_shared<std::vector<ParaType>>();
			auto temp = std::string(val->cbegin(),val->cbegin()+1);
			
			for(auto it = val->cbegin() + 1; it != val->cend();++it)
			{
				if(temp.at(0) == *it)
					temp +=*it;
				else
				{
					ret->push_back(std::make_shared<std::string>(temp.cbegin(), temp.cend()));
					temp = std::string(it,it+1);
				}
			}

			ret->push_back(std::make_shared<std::string>(temp.cbegin(), temp.cend()));
			
			return ret;
		}
	private:
		void setNext(BasePtrType ptr){	this->next = ptr; 	}
		void setCounter(uint c)	{ this->ctr = c; }
	};        
}

#endif
