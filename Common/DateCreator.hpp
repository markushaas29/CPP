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

template<typename T>
class Creator
{
public:
	using Type = T;
	static Type Today()
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return Type((uint)(now->tm_mday), (uint)(now->tm_mon + 1), (uint)(now->tm_year + 1900));			
	} 
	
//	static Type Create(std::istream& is, std::ostream& os) 
//	{
//		auto dt = Today();
//		os<<Identifier<<dt<<std::endl;
//		return Create(is);
//	};

	static Type Create(std::istream& is)
	{
		std::string d;
		is>>d;
		return Type{d};
	}

	static std::string TimeString(const auto& tp)
	{
		std::time_t t = std::chrono::system_clock::to_time_t(tp);
		std::string ts = ctime(&t);
		ts.resize(ts.size()-1);
		return ts;
	}
private:
	//static TupleType createTuple(const std::string& d, const std::string& m, const std::string& y)	{	return std::tuple<Day,Month,Year>(Day(::To<uint>(d)),Month(::To<uint>(m)),Year(::To<uint>(y))); }
};


