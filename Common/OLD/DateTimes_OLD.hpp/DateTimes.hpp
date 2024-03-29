#include <memory>
#include <ratio>
#include <chrono>
#include <ctime>
#include <tuple>
#include <charconv>
#include <array>
#include "../CSV/Element.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../String/StringParser.hpp"
#include "../String/To/To.hpp"
#include "../Validator/Validator.hpp"

#pragma once

static std::array<char,512> Chars(uint d = 0, uint m = 0, uint y = 0)
{
	std::array<char,512> result= {};
	auto day = IntToChars<2>(d);
	auto month = IntToChars<2>(m);
	auto year1 = IntToChars<2>(20);
	auto year = IntToChars<2>(y);
		
		std::cout<<"D "<<d<<std::endl;	
		std::cout<<"M "<<m<<std::endl;	
		std::cout<<"Y "<<y<<std::endl;	
			
			//~ result[0] = year1[0];
			//~ result[1] = year[1];
			//~ result[2] = year[0];
			//~ result[3] = year[1];
			//~ result[4] = '\0';
			result[0] = month[0];
			result[1] = month[1];
			result[2] = year[0];
			result[3] = year[1];
			result[4] = '\0';
			
			//~ std::array<char,512> result = {day[0], day[1], month[0], month[1], year1[0], year[1], year[0], year[1],'\0'};
			return result;
		}


namespace Parsers
{
	template<typename, typename, typename>
	struct Parser;
}

namespace DateTimes
{
	template<class T, size_t N>
	using c_array = T[N];

	template<class T, size_t N>
	constexpr c_array<T, N>& as_c_array(std::array<T, N> a) {
		return reinterpret_cast<T(&)[N]>(*a.data());
	}
	
	template<typename T, typename TChrono, uint Max = 3000, uint Min = 1, typename TChronoVal = uint>
	class DateTimeBase
	{
	public:
		using Derived = T;
		using ChronoType = TChrono;
		using ChronoValueType = TChronoVal;
		constexpr Derived Next() const { return T{this->value + 1}; };
		constexpr Derived Prev() const { return T{this->value - 1}; };
		static constexpr Derived Get(uint i) { return Derived{i};}
		static constexpr Derived Get(int i) { return Derived((uint)i);}
		std::string ToString() const { return String_::ParseTo(value); };
		constexpr uint Value() const { return this->value; }
		constexpr DateTimeBase(uint v):value {RangeValidator<uint,min,max>::Check(v)}, chronoValue{(ChronoValueType)v}
		{
			if(v > max || v == 0)
				Logger::Log<Error>("Value",v," is invalid for",Derived::TypeIdentifier);
		}
		constexpr operator uint() const { return this->value; }
		constexpr operator ChronoType() const { return chronoValue;}
		std::ostream& Display(std::ostream& os) { return os<<value; }
	protected:
		const uint value;
	private:
		template<typename T1, typename T2> friend decltype(auto) operator==(const DateTimes::DateTimeBase<T1,T2>& d1, const DateTimes::DateTimeBase<T1,T2>& d2);
		template<typename T1, typename T2> friend decltype(auto) operator<=>(const DateTimes::DateTimeBase<T1,T2>& d1, const DateTimes::DateTimeBase<T1,T2>& d2);
		const ChronoType chronoValue;
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
		constexpr Month(int v): Base(v){};
	};
	
	class Year: public DateTimeBase<Year, std::chrono::year,3000, 1900, int>
	{
	public:
		using Base = DateTimeBase<Year,std::chrono::year,3000,1900, int>;
		static constexpr const char* TypeIdentifier = "Year";
		constexpr Year(uint v): Base(v), IsLeap(((std::chrono::year)(*this)).is_leap()){};
		bool IsLeap;
	};
		
	bool operator<(const Year& y1, const Year& y2) { return y1.Value() < y2.Value(); };
	
	inline static Month January = Month(1);
	inline static Month February= Month(2);
	inline static Month March= Month(3);
	inline static Month April= Month(4);
	inline static Month May= Month(5);
	inline static Month June= Month(6);
	inline static Month July= Month(7);
	inline static Month August= Month(8);
	inline static Month September= Month(9);
	inline static Month October= Month(10);
	inline static Month November= Month(11);
	inline static Month December= Month(12);
	
	using TP = std::chrono::system_clock::time_point;
	
	template<typename T>
	constexpr bool isYMD() {	return std::is_same_v<T,Day> || std::is_same_v<T,Month> || std::is_same_v<T,Year>;	}
	
	class Date: public Element<Date>
	{
		using Base = Element<Date>;
		friend class Element<Date>;
		
		template<typename ItemT>
		friend const ItemT& Get(Date const& t);
		
		template<typename, typename, typename>
		friend class Parsers::Parser;
	public:
		using DayType = DateTimes::Day;
        using MonthType = DateTimes::Month;
        using YearType = DateTimes::Year;
		using TupleType = std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>;
		using Type = DateTimes::Date;
		inline static constexpr const char* Identifier = "Date";
						
		constexpr Date(uint d = 0, uint m = 0, uint y = 0): 
			Element(getChars(d,m,y)), 
			tt{std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day(d),DateTimes::Month(m),DateTimes::Year(y))},
			ymd{DateTimes::Year{y}, DateTimes::Month{m}, DateTimes::Day{d}}{	}; 
		Date(std::string s, uint d = 0, uint m = 0, uint y = 0): Element{s.c_str()}, tt{extract(s)}, ymd{std::get<DateTimes::Year>(tt), std::get<DateTimes::Month>(tt), std::get<DateTimes::Day>(tt) }{    };
		Date(const std::string& s, const TupleType& t): Date(s.c_str(),  std::get<DateTimes::Day>(t).Value(),  std::get<DateTimes::Month>(t).Value(),  std::get<DateTimes::Year>(t).Value() ) { };
		Date(): Date("", 0,0, 0) { };
		Date* DoCreate(){return this;};

		std::string TimeString()
		{
			std::time_t t = std::chrono::system_clock::to_time_t(this->tp);
			std::string ts = ctime(&t);
			ts.resize(ts.size()-1);
			return ts;
		}
		
		static Date Today()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			return Date((uint)(now->tm_mday), (uint)(now->tm_mon + 1), (uint)(now->tm_year + 1900));			
		} 
		
		std::ostream& Display(std::ostream& out) const {	return out<<std::get<DateTimes::Day>(tt).Value()<<"."<<std::get<DateTimes::Month>(tt).Value()<<"."<<std::get<DateTimes::Year>(tt).Value();	}

		const std::string Value() const  {	return converter(std::get<DateTimes::Day>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Month>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Year>(this->tt).Value()); }
		
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
		TupleType tt;
		const std::chrono::year_month_day ymd;
		TP tp;
		String_::ParserFrom<uint> converter;
		friend decltype(auto) operator<=>(const DateTimes::Date& d1, const DateTimes::Date& d2) noexcept { return d1.ymd <=> d2.ymd; }; 
		static TupleType extract(const std::string& s)
		{
			auto it = std::find_if(s.cbegin(),s.cend(),[](auto c){ return !isdigit(c); });
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
										
			for(auto c : s)
				if(isdigit(c))
					res += c;
			
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
		static constexpr std::array<char,512> getChars(uint d = 0, uint m = 0, uint y = 0)
		{
			std::array<char,512> result= {};
			//~ auto day = IntToChars<2>(d);
			//~ auto month = IntToChars<2>(m);
			//~ auto year1 = IntToChars<2>(20);
			//~ auto year = IntToChars<2>(y);
			
			//~ result[0] = day[0];
			//~ result[1] = day[1];
			//~ result[2] = month[0];
			//~ result[3] = month[1];
			//~ result[4] = year1[0];
			//~ result[5] = year[1];
			//~ result[6] = year[0];
			//~ result[7] = year[1];
			//~ result[8] = '\0';
			
			return result;
		}
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

namespace Parsers
{	
	template<>
	struct Parser<std::string, DateTimes::Date,std::string>
	{
		static DateTimes::Date Parse(std::string s) { return DateTimes::Date(s, DateTimes::Date::extract(s)); }
	};
}

