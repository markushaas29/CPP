#include <memory>
#include <ratio>
#include <chrono>
#include <ctime>
#include <string>
#include <tuple>
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
	template<typename ItemT> friend const ItemT& Get(Date const& t);
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
		return Create(is);
	};

	static Type Create(std::istream& is)
	{
		std::string d;
		is>>d;
		return Type{d};
	}

	std::string TimeString()
	{
		std::time_t t = std::chrono::system_clock::to_time_t(this->tp);
		std::string ts = ctime(&t);
		ts.resize(ts.size()-1);
		return ts;
	}

	virtual const std::regex Pattern() const { return pattern; };

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
		return Date(day,month,year); 
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
	std::regex pattern = std::regex( "(0?[1-9]|[1-2][0-9]|3[0-1]).(0?[1-9]|1[0-2]).(\\d{4})" );
	String_::ParserFrom<uint> converter;
	friend std::ostream& operator<<(std::ostream& out, const Date& d) {	return out<<d.day.Value()<<"."<<d.month.Value()<<"."<<d.year.Value();	}
	friend std::istream& operator>>(std::istream& is, Date& d)
	{	
		std::string s;
		is>>s;
		d = Date{s};
		//d = temp;
		std::cout<<"IS"<<s<<"\t"<<d.day<<std::endl;
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

static decltype(auto) NumberOfDays(const Date& d1, const Date& d2)
{
	const std::chrono::year_month_day ymd1{std::chrono::year(d1.Y()), std::chrono::month(d1.M()), std::chrono::day(d1.D())};
	const std::chrono::year_month_day ymd2{std::chrono::year(d2.Y()), std::chrono::month(d2.M()), std::chrono::day(d2.D())};
	
	if(d1 > d2)
		return Quantity<Time,Days,uint>{static_cast<uint>((std::chrono::sys_days{ymd1} - std::chrono::sys_days{ymd2}).count())};
		
	return Quantity<Time,Days,uint>{static_cast<uint>((std::chrono::sys_days{ymd2} - std::chrono::sys_days{ymd1}).count())};
}

decltype(auto) operator-(const Date& d1, const Date& d2)  { return NumberOfDays(d1,d2); }

