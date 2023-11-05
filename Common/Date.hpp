#include <memory>
#include <ratio>
#include <chrono>
#include <ctime>
#include <string>
#include <tuple>
#include "DateCreator.hpp"
#include "../CSV/Element.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"
#include "../Validator/Validator.hpp"

#pragma once

using TP = std::chrono::system_clock::time_point;

template<typename T>
constexpr bool isYMD() {	return std::is_same_v<T,Day> || std::is_same_v<T,Month> || std::is_same_v<T,Year>;	}

class Date: public Element<Date>
{
	using Base = Element<Date>;
	friend class Element<Date>;
	friend decltype(auto) operator<=>(const Date& d1, const Date& d2) noexcept { return d1.ymd <=> d2.ymd; }; 
public:
	using DayType = Day;
    using MonthType = Month;
    using YearType = Year;
	using TupleType = std::tuple<Day,Month,Year>;
	using Type = Date;
	inline static constexpr const char* Identifier = "Date";
	inline static constexpr const char* Default = "1.1.1900";
					
	Date(Day d, Month m,Year y): 
		Base(d.ToString() + "." + m.ToString() + "." + y.ToString()), day{d}, month{m}, year{y},
		valid{d.Valid() && m.Valid() && y.Valid()},
		ymd{y,m, d}{	}; 
	Date(const std::string& s, const TupleType& t): Date(std::get<Day>(t).Value(),  std::get<Month>(t).Value(),  std::get<Year>(t).Value()) { };
	Date(uint d = 0, uint m = 0, uint y = 0): Date(Day(d),Month(m),Year(y)) {};
	Date(const Date& d): Base{std::string(d.Data().cbegin(),d.Data().cend())}, ymd{d.ymd}, valid{d.valid}, day{d.day}, month{d.month}, year{d.year}, tp{d.tp}, converter{d.converter}  { };
	Date(const std::string& s): Date{check(s.c_str()), extract(check(s.c_str())) }{    };
	
	static Type Create(std::istream& is, std::ostream& os){ return Creator<Date>::create(is,os,Today(), Identifier); } 
	static Type Create(std::istream& is) { return Creator<Date>::create(is); }
	std::string TimeString(const TP& tp) { return Creator<Date>::timeString(tp); }
	static Date Today(){ return Creator<Date>::today(); }

	constexpr bool Valid() const noexcept { return valid && std::chrono::year_month_day(std::chrono::year{1900},std::chrono::month{1},std::chrono::day{1}) != ymd && ymd.ok(); };
	constexpr explicit operator Day() { return day; } 
	constexpr explicit operator Month() { return month; } 
	constexpr explicit operator Year() { return year; } 
	constexpr Day D() const { return day; } 
	constexpr Month M() const { return month; } 
	constexpr Year Y() const { return year; } 
	
	Date operator=(const Date& date)
	{ 
		day = date.day;
		month = date.month;
		year = date.year;
		ymd = date.ymd;
		valid = date.valid;
		tp = date.tp;
		return *this; 
	};
	template<typename T>
	constexpr bool operator==(const T t) const
	{ 
		if constexpr (isYMD<T>())
			return (T)t == std::get<T>(std::make_tuple(day,month,year)); 
		return false;
	};
	constexpr bool operator==(const Date& date) const{ return ymd == date.ymd; };
	constexpr bool operator>(const Date& d) const { return ymd > d.ymd;	}
	constexpr std::strong_ordering operator<=>( const Date& d) noexcept { return ymd <=> d.ymd; }		
private:
	bool valid = false;
	Day day;
	Month month;
	Year year;
	std::chrono::year_month_day ymd;
	TP tp;
	String_::ParserFrom<uint> converter;
	friend decltype(auto) operator-(const Date& d1, const Date& d2);
	friend std::ostream& operator<<(std::ostream& out, const Date& d) {	return out<<d.day.Value()<<"."<<d.month.Value()<<"."<<d.year.Value();	}
	friend std::istream& operator>>(std::istream& is, Date& d)
	{	
		std::string s;
		is>>s;
		d = Date{s};
		return is;
	}
	
	static std::string check(std::string s) 
	{
		if (s.size() == 0) 
			return  Default; 
		for(int i =0; i < s.size(); ++i )
			if(s[i] < 46 || s[i] > 57)
				return  Default;
		return s;
	}
	static TupleType extract(const std::string& s) { return Creator<Date>::extract(s); }
	static TupleType createTuple(const std::string& d, const std::string& m, const std::string& y)	{ return Creator<Date>::createTuple(d,m,y); }
};

decltype(auto) operator-(const Date& d1, const Date& d2)  { return Quantity<Time,Days,uint>{static_cast<uint>((d1 > d2 ? std::chrono::sys_days{d1.ymd} - std::chrono::sys_days{d2.ymd} : std::chrono::sys_days{d2.ymd} - std::chrono::sys_days{d1.ymd}).count())}; }

