#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <locale>
#include <codecvt>
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../Traits/Traits.h"
#include "../Typelist/Typelist.h"

#ifndef STRING_H
#define STRING_H

namespace String_
{
	//---------------------------------------------------------------------------------------------------Split----------------------------------------------------------------------------------------

	using Delimiter = T::char_<'/'> ;
	using Newline = T::char_<'\n'> ;
	using CR = T::char_<'\r'> ;
	using Tab = T::char_<'\t'> ;
	using Space = T::char_<' '> ;
	using Comma = T::char_<','> ;
	using Semicolon = T::char_<';'> ;
	using Point = T::char_<'.'> ;
	
	T::Is_<Delimiter> isDelimiter;
	T::IsNot_<Delimiter> isNotDelimiter;
		
	template<typename Type = Delimiter>
	std::vector<std::string> Split(const std::string strT)
	{
		std::vector<std::string> result;		
		auto token = std::string(1,Type::Value);
		auto str = std::string(strT);
		
		while(str.size())
		{
			int index = str.find(token);
			if(index!=std::string::npos)
			{
				result.push_back(str.substr(0,index));
				str = str.substr(index+token.size());
				
				if(str.size()==0)
					result.push_back(str);
			}
			else
			{
				result.push_back(str);
				str = "";
			}
		}
		return result;
	};
	
	bool Contains(const std::string& s, const std::string& sub)
	{
		return s.find(sub) != std::string::npos; 
	}
	
	std::wstring utf8_to_wstring(const std::string& s)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(s);
	}
	
	std::wstring to_wstring(const std::string& s, const std::locale& loc= std::locale())
	{
		std::vector<wchar_t> buf(s.size());
		std::use_facet<std::ctype<wchar_t>>(loc).widen(s.data(), s.data() + s.size(), buf.data());
		return std::wstring(buf.data(),buf.size());
	}
	
	template<typename T = char>
	bool Compare(const std::string& s1, const std::string& s2)
	{
		std::locale loc("de_DE.UTF-8");
		const std::collate<T>& col = std::use_facet<std::collate<T>>(loc);		
		auto w1 = std::wstring(s1.begin(), s1.end());
		
		return s1 == s2; 
	}
	
	template<typename T>
	std::string Remove(const std::string& s)
	{
		std::string result = s;
		result.erase(std::remove(result.begin(), result.end(), T::Value), result.end());
		
		return result;
	}
		
	template<typename From, typename To>
	struct Replace
	{
		std::string operator()(const std::string& s) const 
		{
			auto result = std::string(s);
			std::replace(result.begin(), result.end(), From::Value, To::Value);
			
			return result;
		}	
	};
	
	using CommaToPoint = Replace<Comma,Point>;

	template<typename>	struct To;
	
	template<typename T>
	struct ToBase
	{
		using Type = T;
		
		T operator()(const std::string& s) const 
		{ 
			try
			{
				 return To<T>::Cast(s);
			}
			catch(const std::exception& e)
			{
				Logger::Log<Error>()<<"Cant cast from "<<s<<" to "<<typeid(T).name()<<std::endl;
				Logger::Log<Error>()<<e.what()<<std::endl;
			}
			
			return T{};
		}
	};
	
	template<typename T = int>
	struct To: public ToBase<T>
	{
		T static Cast(const std::string& s)  { return std::stoi(s); }
	};
	
	template<>
	struct To<double>: public ToBase<double>
	{
		double static Cast(const std::string& s) { return std::stod(s); }
	};
	
	template<>
	struct To<unsigned>: public ToBase<unsigned>
	{
		unsigned static Cast(const std::string& s)  { return std::stol(s); }
	};
	
	template<typename T>
	struct From
	{
		std::string operator()(T t) { return ""; }
	};
	
	template<>
	struct From<int>
	{
		std::string operator()(int t) { return std::to_string(t); }
	};
	
	template<>
	struct From<uint>
	{
		std::string operator()(uint t) { return std::to_string(t); }
	};
	
	template<>
	struct From<double>
	{
		std::string operator()(double t) { return std::to_string(t); }
	};
	
	static From<int> FromInt = From<int>();
}

#endif
