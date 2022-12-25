#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "SpecialAtomsContainer.hpp"
#include "QuantityContainer.hpp"
#include "ParsedValue.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Common/Number.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef PARSEDVALUES_HPP
#define PARSEDVALUES_HPP

namespace String_
{
	class ParsedWord: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedWord>;
	public:
		static constexpr const char* Key = "Word";
		ParsedWord(ParsedValue::ParaType val, uint start = 0, uint end = 0, uint c = 0, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, start, end , c, next){	Logger::Log()<<"VAL Constructor Word: "<<*val<<std::endl;}
		
		std::string Cast(){ return "Test"; }
	};
	
	class ParsedBlank: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedBlank>;
	public:
		static constexpr const char* Key = "Blank";
		ParsedBlank(ParsedValue::ParaType val,  uint start = 0, uint end = 0, uint c = 0, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, start, end , c, next)	{	Logger::Log()<<"VAL Constructor Blank: "<<*val<<std::endl;	}
		
		std::string Value(){ return Key; }
	};
	
	class ParsedSpace: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedSpace>;
	public:
		static constexpr const char* Key = "Space";
		ParsedSpace(ParsedValue::ParaType val, uint start = 0, uint end = 0, uint c = 0, ParsedValue::BasePtrType next = nullptr): ParsedValue(val, start, end , c, next){}
		
		std::string Cast(){ return Key; }
	};        
	
	template<typename T>
	class SpecializedPunctation;
	
	class ParsedPunct: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedPunct>;
		using ReturnType = ReturnT<ParsedPunct>;
		using Types = Typelist<T::char_<'.'>,T::char_<','>,  T::char_<'!'>, T::char_<'"'>,T::char_<'#'>,T::char_<'$'>,T::char_<'%'>,T::char_<'&'>,T::char_<'\''>,T::char_<'<'>,
		T::char_<'('>,T::char_<')'>,T::char_<'*'>,T::char_<'+'>,T::char_<'-'>,T::char_<'/'>,T::char_<':'>,T::char_<';'>,T::char_<'<'>,T::char_<'='>,T::char_<'?'>,
		T::char_<'@'>,T::char_<'['>,T::char_<'\\'>,T::char_<']'>,T::char_<'^'>,T::char_<'_'>,T::char_<'`'>,T::char_<'{'>,T::char_<'|'>,T::char_<'}'>, T::char_<'~'>>;
		
		using SpecialAtomContainerType = SpecialAtomContainer<ParsedPunct, Types>::ContainerType;
	protected:
		std::function<bool(char)> isImpl;
	public:
		static constexpr const char* Key = "Punct";
		ParsedPunct(ParsedValue::ParaType val, std::function<bool(char)> isPara, uint start = 0, uint end = 0, uint c = 0, ParsedValue::BasePtrType next = nullptr): isImpl(isPara),ParsedValue(val, start, end , c, next){	}
		
		static auto Create(ParsedValue::ParaType p)
		{
			auto commons = extractCommonAtoms(p);
			ReturnType ret = std::make_shared<std::vector<PtrType>>();
			for(auto it = commons->cbegin(); it != commons->cend(); ++it)
				ret->push_back(SpecialAtomContainerType::Instance().Parse(*it));
			return ret;
		}
		
		template<typename P>
		bool Is() { return this->isImpl(P::Value); }

		std::string Cast(){ return "Test"; }
	};
	
	class ParsedNumber: public ParsedValue
	{
		using PtrType = std::shared_ptr<ParsedNumber>;
		using ValuesType = std::shared_ptr<std::vector<uint>>;
		inline static auto ZERO = T::char_<'0'>(); 
		ValuesType vals;		
		using Types = Typelist<Quantity<Sum>,Quantity<Length>,Quantity<Mass>,Quantity<Time>,Quantity<Current>,Quantity<Temperature>, Quantity<Volume>,Quantity<Work>,Quantity<Area>,Quantity<Scalar>>;		
	public:
		static constexpr const char* Key = "Number";
		using QuantityContainerType = QuantityContainer<Types>::ContainerType;
		ParsedNumber(ParsedValue::ParaType val, uint start = 0, uint end = 0, uint c = 0, ParsedValue::BasePtrType next = nullptr): 
			vals(std::make_shared<std::vector<uint>>()), 	ParsedValue(val, start, end , c, next)
		{
			if(val->size() > 0)
			{
				for(auto  i = (val->size())-1;i > 0; --i)
					vals->push_back((uint)(val->at(i) - ZERO));

				vals->push_back((uint)(val->at(0) - ZERO));
			}				
		}
		
		decltype(auto) GetQuantityUnit(){ return QuantityContainerType::Instance().Get(this->Cast<int>(),this->next->Value());	}
		decltype(auto) AsQuantity()	{ return QuantityContainerType::Instance().Add(this->Cast<int>(),this->next->Value());	}
		
		template<typename Q>
		static decltype(auto) Get()	{	return QuantityContainerType::Instance().Get<Q>();	}
				
		ValuesType Values() { return this->vals; }		
		uint operator[](uint i)	const {	return (uint)(this->vals->at(i) );	}
		
        bool IsCastable()
        {
			constexpr auto max = std::numeric_limits<long>::max();
			uint digitsMax = Number::Digits<max>::Value;
			
			std::ostringstream os;
			os << max;
			std::string digits = os.str();
			
			if(this->Size() < digitsMax) return true;
			
			if(this->Size() == digitsMax)
			{
				for(auto i = digitsMax; i >= 0; --i)
					if(this->strValue->at(i) > digits.at(i))
						return false;
							
				return true;	
			}			
			
			return false;				
		}
		
		bool operator==(const ParsedNumber& pi)
		{
			if(this->Size() != pi.Size())
				return false;
				
			for(auto i = 0; i < this->Size(); ++i)
				if(this->strValue->at(i) != pi[i])
					return false;
					
			return true;
		}
		
		bool operator<(const ParsedNumber& pi)
		{
			if(this->Size() >= pi.Size())
				return false;
				
			for(auto i = 0; i < this->vals->size(); ++i)
				if(this->strValue->at(i) != pi[i])
					return false;
					
			return true;
		}
		
		static std::ostream& Display(std::ostream& os) { return QuantityContainerType::Display(os);	}
	};	
}

#endif
