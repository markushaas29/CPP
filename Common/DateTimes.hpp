#include <memory>
#include <ratio>
#include <chrono>
#include <tuple>
#include <charconv>
#include <array>
#include "../CSV/Element.hpp"
#include "../String/String_.hpp"

#ifndef DATETIMES_HPP
#define DATETIMES_HPP

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

namespace DateTimes
{
	template<class T, size_t N>
	using c_array = T[N];

	template<class T, size_t N>
	constexpr c_array<T, N>& as_c_array(std::array<T, N> a) {
		return reinterpret_cast<T(&)[N]>(*a.data());
	}
	
	template<typename T, uint mx = 3000, uint mn = 1>
	struct DateTimeBase
	{
		using Derived = T;
		Derived Next() const { return T{this->value + 1}; };
		Derived Prev() const { return T{this->value - 1}; };
		std::string ToString() const { return ""; };
		uint Value() const { return this->value; }
		constexpr DateTimeBase(uint v):value {v < min || v > max ? min : v}
		{
			if(v > max || v == 0)
				Logger::Log<Error>("Value",v," is invalid for",Derived::TypeIdentifier);
		}
		operator uint() const { return this->value; }
	protected:
		const uint value;
	private:
		constexpr static uint min = mn;
		constexpr static uint max = mx;
	};
	
	
	struct Day: DateTimeBase<Day,31>
	{
		using Base = DateTimeBase<Day,31>;
		static constexpr const char* TypeIdentifier = "Day";
		static Base::Derived Get(uint i) { return Day{i};}
		static Base::Derived Get(int i) { return Day((uint)i);}
		constexpr Day(uint v): Base(v){};
		bool operator==(const DateTimes::Day& d) const{ return this->value == d.value; };
		bool operator>(const DateTimes::Day& d) const{ return this->value > d.value; };
	};

	struct Month: DateTimeBase<Month,12>
	{
		using Base = DateTimeBase<Month,12>;
		static constexpr const char* TypeIdentifier = "Month";
		static Base::Derived Get(uint i) { return Month{i};}
		constexpr Month(uint v): Base(v){};
		constexpr Month(int v): Base(v){};
		bool operator==(const DateTimes::Month& d) const{ return this->value == d.value; };
		bool operator>(const DateTimes::Month& d) const{ return this->value > d.value; };
	private:
		static constexpr uint check(uint i)
		{
			if( i > 12) return 1;
			if( i < 1) return 12;
			return i;
		}
	};
	
	struct Year: DateTimeBase<Year, 3000, 1900>
	{
		using Base = DateTimeBase<Year,3000,1900>;
		static constexpr const char* TypeIdentifier = "Year";
		static Base::Derived Get(uint i) { return Year{i};}
		static Base::Derived Get(int i) { return Year((uint)i);}
		constexpr Year(uint v): Base(v), IsLeapYear(isLeapYear(v)){};
		bool operator==(const DateTimes::Year& d) const{ return this->value == d.value; };
		bool operator>(const DateTimes::Year& d) const{ return this->value > d.value; };
		bool IsLeapYear;
	private:
		static constexpr bool isLeapYear(uint year)
		{
			if (year % 400 == 0) 	return true;
			if (year % 100 == 0)	return false;
			if (year % 4 == 0)		return true;
			return false;
		}
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
	
	class Date: public Element
	{
	public:
		using DayType = DateTimes::Day;
        using MonthType = DateTimes::Month;
        using YearType = DateTimes::Year;
		using TupleType = std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>;
		using Type = DateTimes::Date;
		
		template<typename ItemT>
		friend const ItemT& Get(Date const& t);
		
		inline static const std::string Identifier = "Date";
						
		constexpr Date(uint d = 0, uint m = 0, uint y = 0): Element(getChars(d,m,y)), tt{std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day(d),DateTimes::Month(m),DateTimes::Year(y))}{	}; 
		Date(std::string s, uint d = 0, uint m = 0, uint y = 0): Element{s.c_str()}, tt{Extract(s)}{    };
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
		
		static TupleType Extract(const std::string& s)
		{
			uint d, m, y;
			
			std::string res;
			for(auto c : s)
				if(isdigit(c))
					res += c;
			
			if(res.size() > 0)
			{
				d = std::stoul(std::string(res.begin(),res.begin()+2));
				m = std::stoul(std::string(res.begin()+3,res.begin()+4));
				y = std::stoul(std::string(res.begin()+4,res.begin()+8));
			}
			
			return std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>(DateTimes::Day(d),DateTimes::Month(m),DateTimes::Year(y));
		}
		
		
		const std::string Value() const  {	return converter(std::get<DateTimes::Day>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Month>(this->tt).Value()) 
			+ converter(std::get<DateTimes::Year>(this->tt).Value()); }
		bool operator==(const Date& date) const{ return std::get<DateTimes::Day>(date.tt) == std::get<DateTimes::Day>(this->tt) 
			&& std::get<DateTimes::Month>(date.tt) == std::get<DateTimes::Month>(this->tt) 
			&& std::get<DateTimes::Year>(date.tt) == std::get<DateTimes::Year>(this->tt);};
		bool operator==(const Year& y) const{ return std::get<DateTimes::Year>(this->tt) == y; };
		bool operator>(const Date& d) const { 
			if(std::get<DateTimes::Year>(this->tt) > std::get<DateTimes::Year>(d.tt))
				return true;
			if(std::get<DateTimes::Year>(this->tt) == std::get<DateTimes::Year>(d.tt))
			{
				if(std::get<DateTimes::Month>(this->tt) > std::get<DateTimes::Month>(d.tt))
					return true;
				if(std::get<DateTimes::Month>(this->tt) == std::get<DateTimes::Month>(d.tt) && std::get<DateTimes::Day>(this->tt) > std::get<DateTimes::Day>(d.tt))
					return true;
			}
			return false;
		}
	private:
		static constexpr std::array<char,512> getChars(uint d = 0, uint m = 0, uint y = 0)
		{
			std::array<char,512> result= {};
			auto day = IntToChars<2>(d);
			auto month = IntToChars<2>(m);
			auto year1 = IntToChars<2>(20);
			auto year = IntToChars<2>(y);
			
			result[0] = day[0];
			result[1] = day[1];
			result[2] = month[0];
			result[3] = month[1];
			result[4] = year1[0];
			result[5] = year[1];
			result[6] = year[0];
			result[7] = year[1];
			result[8] = '\0';
			
			return result;
		}
		TupleType tt;
		TP tp;
		String_::From<uint> converter;
	};

	template<typename ItemT>
	const ItemT& Get(Date const& d)	{	return std::get<ItemT>(d.tt);	};
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const DateTimes::DateTimeBase<T>& s){	return out<<s.Value();	}

std::ostream& operator<<(std::ostream& out, const DateTimes::Date& d){	return out<<d.Value();	}

namespace Parsers
{
	template<typename, typename, typename>
	struct Parser;
	
	template<>
	struct Parser<std::string, DateTimes::Date,std::string>
	{
		static DateTimes::Date Parse(std::string s) { return DateTimes::Date(s, DateTimes::Date::Extract(s)); }
	};
}

#endif
