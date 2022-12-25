#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "SpecialAtomsContainer.hpp"
#include "QuantityContainer.hpp"
#include "ParsedValue.hpp"
#include "ParsedValues.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Common/Number.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef CREATOR_HPP
#define CREATOR_HPP

namespace String_
{
	template<typename T>
	struct Creator
	{
		static typename T::ContainerParaType Parse(typename T::ParaType p,  uint start = 0, uint end = 0, uint c = 0)
		{
			auto ret = std::make_shared<typename T::ContainerType>();
			ret->push_back(std::make_shared<T>(p,start,end,c));
			return ret;
		}
	};
	
	template<>
	struct Creator<ParsedPunct>
	{
		static typename ParsedPunct::ContainerParaType Parse(typename ParsedPunct::ParaType p, uint start = 0, uint end = 0, uint c = 0)
		{
			auto ret = std::make_shared<typename ParsedPunct::ContainerType>();
			auto vals = ParsedPunct::Create(p);
			ret->insert(ret->end(), vals->begin(), vals->end());
			return ret;
		}
	};
	
	template<> struct IsImpl<uint,ParsedValue::BasePtrType>{ bool operator ()(ParsedValue::BasePtrType bp){ Logger::Log()<<"UINT"<<std::endl; return true;}	};
	template<> struct IsImpl<double, ParsedValue::BasePtrType>
	{ 
		bool operator ()(ParsedValue::BasePtrType bp)
		{ 
			if(bp->Next() == nullptr) return false;
			if(bp->Next()->Next() == nullptr) return false;
			if(!std::dynamic_pointer_cast<ParsedNumber>(bp->Next()->Next())) return false;

			auto pp = std::dynamic_pointer_cast<ParsedPunct>(bp->Next());
			if(!pp) return false;
			
			return pp->template Is<T::char_<','>>();
		}	
	};
        
}

#endif
