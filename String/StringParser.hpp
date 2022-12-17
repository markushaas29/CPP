#include <string>
#include <cstring>
#include <iostream>
#include <typeinfo>

#ifndef STRINGPARSER_H
#define STRINGPARSER_H

namespace String_
{
	template<typename>	struct ParserTo;
	
	template<typename T>
	struct ParserToBase
	{
		using Type = T;
		
		T operator()(const std::string& s) const 
		{ 
			try
			{
				 return ParserTo<T>::Cast(s);
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
	struct ParserTo: public ParserToBase<T>
	{
		T static Cast(const std::string& s)  { return std::stoi(s); }
	};
	
	template<>
	struct ParserTo<double>: public ParserToBase<double>
	{
		double static Cast(const std::string& s) { return std::stod(s); }
	};
	
	template<>
	struct ParserTo<unsigned>: public ParserToBase<unsigned>
	{
		unsigned static Cast(const std::string& s)  { return std::stol(s); }
	};
	
	template<typename T>
	struct ParserFrom
	{
		std::string operator()(T t) const { return ""; }
	};
	
	template<>
	struct ParserFrom<int>
	{
		std::string operator()(int t) const { return std::to_string(t); }
	};
	
	template<>
	struct ParserFrom<uint>
	{
		std::string operator()(uint t) const { return std::to_string(t); }
	};
	
	template<>
	struct ParserFrom<double>
	{
		std::string operator()(double t) const { return std::to_string(t); }
	};
	
	static ParserFrom<int> FromInt = ParserFrom<int>();
}

#endif
