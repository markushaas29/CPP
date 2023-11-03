#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <locale>
#include <codecvt>
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../Traits/Traits.hpp"

#pragma once

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
		
	
	std::vector<std::string> Split(const std::string& strT, const char token)
	{
		std::vector<std::string> result;	
		auto stringToken = std::string(1,token);	
		auto str = std::string(strT);
		
		while(str.size())
		{
			size_t index = str.find(stringToken);
			if(index!=std::string::npos)
			{
				result.push_back(str.substr(0,index));
				str = str.substr(index+stringToken.size());
				
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
	
	template<typename Type = Delimiter>
	std::vector<std::string> Split(const std::string& strT) { return Split(strT, Type::Value); }
	
	bool Contains(const std::string& s, const std::string& sub)
	{
		return s.find(sub) != std::string::npos; 
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
	
	
	std::string TrimDouble(double d)
	{
		auto s = std::to_string(d);
	
		if(!Contains(s,".") || *(s.cend()-1) != '0')
			return s;

		std::string result;
		if (auto it = std::find_if(begin(s), end(s), [](char c){ return c == '.'; }); it != std::end(s))
			if (auto itp = std::find_if(it, end(s), [](char c){ return c == '0'; }); it != std::end(s))
				std::for_each(s.begin(),itp, [&](auto c) {	result += c; });
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
}
