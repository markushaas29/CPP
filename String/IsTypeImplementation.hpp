#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include "SpecialAtomsContainer.hpp"
#include "PaesedValues.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.h"
#include "../Logger/Logger.hpp"

#ifndef ISTYPEIMPL_HPP
#define ISTYPEIMPL_HPP

namespace String_
{
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
