#include <memory>
#include <ratio>
#include <chrono>
#include <tuple>
#include "../CSV/CSV.hpp"

#ifndef DATETIMES_HPP
#define DATETIMES_HPP

namespace DateTimes
{
	template<typename T>
	struct DateTimeBase
	{
		using Derived = std::shared_ptr<T>;
		Derived Next() const { return std::make_shared<T>(this->value + 1); };
		Derived Prev() const { return std::make_shared<T>(this->value - 1); };
		std::string ToString() const { return ""; };
		uint Value() const { return this->value; }
		constexpr DateTimeBase(uint v):value{v} {}
		operator uint() { return this->value; }

	protected:
		const uint value;
	};

	struct Month: DateTimeBase<Month>
	{
		using Base = DateTimeBase<Month>;
		static Base::Derived Get(uint i) { return std::make_shared<Month>(i);}
		constexpr Month(uint v): DateTimeBase<Month>(c(v)){};
		constexpr Month(int v): DateTimeBase<Month>(c((uint)v)){};
		bool operator==(const DateTimes::Month& d) const{ return this->value == d.value; };
	private:
		constexpr uint c(uint i)
		{
			if( i > 12) return 1;
			if( i < 1) return 12;
			return i;
		}
	};
	
	struct Year: DateTimeBase<Year>
	{
		using Base = DateTimeBase<Year>;
		static Base::Derived Get(uint i) { return std::make_shared<Year>(i);}
		static Base::Derived Get(int i) { return std::make_shared<Year>((uint)i);}
		Year(uint v): DateTimeBase<Year>(v){};
		bool operator==(const DateTimes::Year& d) const{ return this->value == d.value; };
		
	};
	
	struct Day: DateTimeBase<Day>
	{
		using Base = DateTimeBase<Day>;
		static Base::Derived Get(uint i) { return std::make_shared<Day>(i);}
		static Base::Derived Get(int i) { return std::make_shared<Day>((uint)i);}
		Day(uint v): DateTimeBase<Day>(v){};
		bool operator==(const DateTimes::Day& d) const{ return this->value == d.value; };
	};
	
	
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
						
		Date(std::string s, uint d = 0, uint m = 0, uint y = 0): Element{s}, tt{Extract(s)}{	}; 
		Date(const std::string& s, const TupleType& t): Date(s,  std::get<DateTimes::Day>(t).Value(),  std::get<DateTimes::Month>(t).Value(),  std::get<DateTimes::Year>(t).Value() ) { };
		Date(uint d, uint m, uint y): Date(std::string(), d,m, y) { };
		Date(): Date(std::string(), 0,0, 0) { };
		Date* DoCreate(){return this;};

		std::string TimeString()
		{
			std::time_t t = std::chrono::system_clock::to_time_t(this->tp);
			std::string ts = ctime(&t);
			ts.resize(ts.size()-1);
			return ts;
		}
		
		static TupleType Extract(std::string s)
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
		
		bool operator==(const Date& date) const{ return std::get<DateTimes::Day>(date.tt) == std::get<DateTimes::Day>(this->tt) 
			&& std::get<DateTimes::Month>(date.tt) == std::get<DateTimes::Month>(this->tt) 
			&& std::get<DateTimes::Year>(date.tt) == std::get<DateTimes::Year>(this->tt);};
	private:
		TupleType tt;
		TP tp;
	};

	template<typename ItemT>
	const ItemT& Get(Date const& d)
	{
		return std::get<ItemT>(d.tt);
	};
}

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
