#include <memory>
#include <ratio>
#include <chrono>
#include <ctime>
#include <string>
#include <tuple>
#include "DateTimeBase.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"
#include "../Validator/Validator.hpp"

#pragma once

class Date;

template<typename T>
class Creator
{
public:
	using Type = T;
private:
	friend class Date;
	using TupleType = typename T::TupleType;
	static Type today()
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return Type((uint)(now->tm_mday), (uint)(now->tm_mon + 1), (uint)(now->tm_year + 1900));			
	} 
	
	static Type create(std::istream& is, std::ostream& os, const Type& dt, const char* identifier) 
	{
		os<<identifier<<dt<<std::endl;
		return create(is);
	};

	static Type create(std::istream& is)
	{
		std::string d;
		is>>d;
		return Type{d};
	}

	static std::string timeString(const auto& tp)
	{
		std::time_t t = std::chrono::system_clock::to_time_t(tp);
		std::string ts = ctime(&t);
		ts.resize(ts.size()-1);
		return ts;
	}
	static TupleType extract(const std::string& s)
	{
		auto it = std::find_if(s.cbegin(),s.cend(),[](auto c){ return !isdigit(c); });
		auto is = std::find_if(s.cbegin(),s.cend(),[](auto c){ return isspace(c); });
		if(it != s.cend())
			return extractBySeparation(s,*it);
		else
			return extractByValue(s);				
	}
	static TupleType extractBySeparation(const std::string& s, const char token)
	{
		auto values = String_::Split(s,token);
		
		if(values.size() == 3)
			return createTuple(values.at(0),values.at(1),values.at(2));
		return std::tuple<Day,Month,Year>(Day{1},Month{1},Year{1900});
	}
	static TupleType extractByValue(const std::string& s)
	{
		std::string res;
		std::for_each(s.cbegin(), s.cend(),[&](auto c) 
			{ if(isdigit(c))
				res += c;	});
		
		if(res.size() > 0)
		{
			if(res.size() < 8)
				Logger::Log<Warning>("Date: ", s, " separated by value and might not be valid!");  
			return createTuple(std::string(res.begin(),res.begin()+2), std::string(res.begin()+2,res.begin()+4),std::string(res.begin()+4,res.begin()+8));
			
		}
		return std::tuple<Day,Month,Year>(Day{1},Month{1},Year{1900});			
	}
	
	static TupleType createTuple(const std::string& d, const std::string& m, const std::string& y)	{	return std::tuple<Day,Month,Year>(Day(::To<uint>(d)),Month(::To<uint>(m)),Year(::To<uint>(y))); }
};


