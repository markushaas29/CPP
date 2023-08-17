#include <memory>
#include <ratio>
#include <chrono>
#include <ctime>
#include <string>
#include <tuple>
#include <charconv>
#include <array>
#include "../CSV/Element.hpp"
#include "Make/Make.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../String/StringParser.hpp"
#include "../String/To/To.hpp"
#include "../Validator/Validator.hpp"

#pragma once

namespace Parsers{	template<typename, typename, typename>	struct Parser;}

namespace DateTimes
{
	template<typename T, typename TChrono, uint Max = 3000, uint Min = 1, typename TChronoVal = uint, typename TValue = TChronoVal>
	class DateTimeBase
	{
	public:
		using Derived = T;
		using ValueType = TValue;
		using ChronoType = TChrono;
		using ChronoValueType = TChronoVal;
		constexpr Derived Next() const { return T{value + 1}; };
		constexpr Derived Prev() const { return T{value - 1}; };
		static constexpr Derived Get(uint i) { return Derived{i};}
		static constexpr Derived Get(int i) { return Derived((uint)i);}
		std::string ToString() const { return std::to_string(value); };
		constexpr uint Value() const { return value; }
		constexpr DateTimeBase(uint v):value {RangeValidator<uint,min,max>::Check(v)}, valid{RangeValidator<uint,min,max>::Condition(v)}, chronoValue{(ChronoValueType)v}
		{
			if(v > max || v < min || v == 0)
				Logger::Log<Error>("Value",v," is invalid for",Derived::TypeIdentifier);
		}
		constexpr bool Valid() const { return valid; }
		constexpr operator uint() const { return value; }
		constexpr operator ChronoType() const { return chronoValue;}
		static decltype(auto) Create(std::istream& is, std::ostream& os)
		{
			os<<Derived::TypeIdentifier<<": ";
			return Create(is);
		}
		static decltype(auto) Create(std::istream& is)
		{
			ValueType cv;
			is>>cv;
			if(!is && !RangeValidator<uint,min,max>::Condition(cv))
				Logger::Log<Error>("Invalid Input",cv);		
			return Derived{cv};
		}
		std::ostream& Display(std::ostream& os) { return os<<value; }
	protected:
		const uint value;
	private:
		template<typename T1, typename T2> friend decltype(auto) operator==(const DateTimes::DateTimeBase<T1,T2>& d1, const DateTimes::DateTimeBase<T1,T2>& d2);
		template<typename T1, typename T2> friend decltype(auto) operator<=>(const DateTimes::DateTimeBase<T1,T2>& d1, const DateTimes::DateTimeBase<T1,T2>& d2);
		const ChronoType chronoValue;
		const bool valid;
		constexpr static uint min = Min;
		constexpr static uint max = Max;
	};
	
	class Day: public DateTimeBase<Day,std::chrono::day,31>
	{
	public:
		using Base = DateTimeBase<Day,std::chrono::day,31>;
		static constexpr const char* TypeIdentifier = "Day";
		constexpr Day(uint v): Base(v){};
	};
		
	class Month: public DateTimeBase<Month,std::chrono::month,12>
	{
	public:
		using Base = DateTimeBase<Month,std::chrono::month,12>;
		static constexpr const char* TypeIdentifier = "Month";
		constexpr Month(uint v): Base(v){};
	};
	
	class Year: public DateTimeBase<Year, std::chrono::year,3000, 1900, int, uint>
	{
	public:
		using Base = DateTimeBase<Year,std::chrono::year,3000,1900, int, uint>;
		static constexpr const char* TypeIdentifier = "Year";
		constexpr Year(uint v): Base(v), IsLeap(((std::chrono::year)(*this)).is_leap()){};
		bool IsLeap;
	};
		
	bool operator<(const Year& y1, const Year& y2) { return y1.Value() < y2.Value(); };
	
	inline constexpr static Month January = Month(1);
	inline constexpr static Month February= Month(2);
	inline constexpr static Month March= Month(3);
	inline constexpr static Month April= Month(4);
	inline constexpr static Month May= Month(5);
	inline constexpr static Month June= Month(6);
	inline constexpr static Month July= Month(7);
	inline constexpr static Month August= Month(8);
	inline constexpr static Month September= Month(9);
	inline constexpr static Month October= Month(10);
	inline constexpr static Month November= Month(11);
	inline constexpr static Month December= Month(12);
	
	using TP = std::chrono::system_clock::time_point;
	
	template<typename T>
	constexpr bool isYMD() {	return std::is_same_v<T,Day> || std::is_same_v<T,Month> || std::is_same_v<T,Year>;	}
	
	class Date: public Element<Date>
	{
		using Base = Element<Date>;
		friend class Element<Date>;
		friend decltype(auto) operator<=>(const DateTimes::Date& d1, const DateTimes::Date& d2) noexcept { return d1.ymd <=> d2.ymd; }; 
		template<typename ItemT> friend const ItemT& Get(Date const& t);
		template<typename, typename, typename>	friend class Parsers::Parser;
	public:
		using DayType = DateTimes::Day;
        using MonthType = DateTimes::Month;
        using YearType = DateTimes::Year;
		using TupleType = std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>;
		using Type = DateTimes::Date;
		inline static constexpr const char* Identifier = "Date";
						
		constexpr Date(DateTimes::Day d, DateTimes::Month m,DateTimes::Year y, const char* t = ""): 
			valid{d.Valid() && m.Valid() && y.Valid()},
			Element(t), 
			tt{std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(d,m,y)},
			ymd{y,m, d}{	}; 
		constexpr Date(uint d = 0, uint m = 0, uint y = 0): Date(DateTimes::Day(d),DateTimes::Month(m),DateTimes::Year(y)) {};
		constexpr Date(const char* e, const TupleType& t): Date(std::get<DateTimes::Day>(t).Value(),  std::get<DateTimes::Month>(t).Value(),  std::get<DateTimes::Year>(t).Value(), e) { };
		Date(const Date& d): Element{d.Value().c_str()}, ymd{d.ymd}, valid{d.valid}, tt{d.tt}, tp{d.tp}, converter{d.converter}  { };
		Date(const std::string& s): Date{s.c_str(), extract(s) }{    };
		Date* DoCreate(){return this;};
		
		static Date Today()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			return Date((uint)(now->tm_mday), (uint)(now->tm_mon + 1), (uint)(now->tm_year + 1900));			
		} 
		
		static Type Create(std::istream& is, std::ostream& os) 
		{
			auto dt = Today();
			os<<Identifier<<dt<<std::endl;
			dt.Display(os);
			return Create(is);
		};

		static Type Create(std::istream& is)
		{
			std::string d;
			is>>d;
			
			return Type{d};
		}
		

		std::ostream& Display(std::ostream& out) const {	return out<<std::get<DateTimes::Day>(tt).Value()<<"."<<std::get<DateTimes::Month>(tt).Value()<<"."<<std::get<DateTimes::Year>(tt).Value();	}

		const std::string Value() const  {	return converter(std::get<DateTimes::Day>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Month>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Year>(this->tt).Value()); }
		
		std::string TimeString()
		{
			std::time_t t = std::chrono::system_clock::to_time_t(this->tp);
			std::string ts = ctime(&t);
			ts.resize(ts.size()-1);
			return ts;
		}

		constexpr bool Valid() const noexcept { return valid && ymd.ok(); };
		constexpr explicit operator Day() { return std::get<DateTimes::Day>(tt); } 
		constexpr explicit operator Month() { return std::get<DateTimes::Month>(tt); } 
		constexpr explicit operator Year() { return std::get<DateTimes::Year>(tt); } 
		
		template<typename T>
		constexpr bool operator==(const T t) const
		{ 
			if constexpr (isYMD<T>())
				return (T)t == std::get<T>(tt); 
			return false;
		};
		constexpr bool operator==(const Date& date) const{ return ymd == date.ymd; };
		constexpr bool operator>(const Date& d) const { return ymd > d.ymd;	}
		constexpr std::strong_ordering operator<=>( const Date& d) noexcept { return ymd <=> d.ymd; }		
	private:
		bool valid = false;
		TupleType tt;
		const std::chrono::year_month_day ymd;
		TP tp;
		String_::ParserFrom<uint> converter;
		
		static TupleType extract(const std::string& s)
		{
			auto it = std::find_if(s.cbegin(),s.cend(),[](auto c){ return !isdigit(c); });
			auto is = std::find_if(s.cbegin(),s.cend(),[](auto c){ return isspace(c); });
			//~ std::cout<<"BYVAL:"<<*it<<"END"<<std::endl;
			//~ if(is != s.cend())
				//~ return extractBySeparation(s,' ');
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
			return std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day{1},DateTimes::Month{1},DateTimes::Year{1900});
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
			return std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day{1},DateTimes::Month{1},DateTimes::Year{1900});			
		}
		
		static TupleType createTuple(const std::string& d, const std::string& m, const std::string& y)
		{
			return std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day(String_::ParseTo<uint>(d)),DateTimes::Month(String_::ParseTo<uint>(m)),DateTimes::Year(String_::ParseTo<uint>(y)));
		}
		
		static constexpr const char* check(const char* s) { return s; }
	};

	static decltype(auto) NumberOfDays(const Date& d1, const Date& d2)
	{
		const std::chrono::year_month_day ymd1{std::chrono::year(Get<Year>(d1)), std::chrono::month(Get<Month>(d1)), std::chrono::day(Get<Day>(d1))};
		const std::chrono::year_month_day ymd2{std::chrono::year(Get<Year>(d2)), std::chrono::month(Get<Month>(d2)), std::chrono::day(Get<Day>(d2))};
		
		if(d1 > d2)
			return Quantity<Time,Days,uint>{static_cast<uint>((std::chrono::sys_days{ymd1} - std::chrono::sys_days{ymd2}).count())};
			
		return Quantity<Time,Days,uint>{static_cast<uint>((std::chrono::sys_days{ymd2} - std::chrono::sys_days{ymd1}).count())};
	}
	
	template<typename ItemT>
	const ItemT& Get(Date const& d)	{	return std::get<ItemT>(d.tt);	};
}

decltype(auto) operator-(const DateTimes::Date& d1, const DateTimes::Date& d2)  { return DateTimes::NumberOfDays(d1,d2); }
template<typename T, typename TC>
decltype(auto) operator==(const DateTimes::DateTimeBase<T,TC>& d1, const DateTimes::DateTimeBase<T,TC>& d2){ return d1.chronoValue == d2.chronoValue;	}

template<typename T, typename TC>
decltype(auto) operator<=>(const DateTimes::DateTimeBase<T,TC>& d1, const DateTimes::DateTimeBase<T,TC>& d2){ return d1.chronoValue <=> d2.chronoValue;	}

template<typename T, typename TC>
std::ostream& operator<<(std::ostream& out, const DateTimes::DateTimeBase<T,TC>& s){	return out<<s.Value();	}

std::ostream& operator<<(std::ostream& out, const DateTimes::Date& d){	return d.Display(out);	}

template<>
struct TryMake<DateTimes::Date>
{
decltype(auto) operator()(std::istream& arg)
{
	using Target = DateTimes::Date;
	auto result = DateTimes::Date::Create(arg);
	if(!result.Valid())
		return ParseResult<Target>();
	return ParseResult<Target>(result);
}
};
template<>
struct Make<DateTimes::Date>
{
decltype(auto) operator()(std::istream& is)
{
	auto tm = TryMake<DateTimes::Date>();
	auto result = tm(is);
	if(!result.Valid)
		throw std::runtime_error("Make() failed");
	return result;
}
};
namespace Parsers
{	
	template<>
	struct Parser<std::string, DateTimes::Date,std::string>
	{
		static DateTimes::Date Parse(std::string s) { return DateTimes::Date(s); }
	};
}

