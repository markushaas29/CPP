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
		bool operator==(uint i){ return this->value == i;};
		bool operator==(int i){ return this->value == i;};
		bool operator==(T t){ return this->value == t.Value();};
		bool operator==(Derived d){ return this->value == d->Value();};

		bool operator<(T t) const{ return this->value < t.Value();};
		bool operator>(T t) const{ return this->value > t.Value();};
	protected:
		const uint value;
	};

	struct Month: DateTimeBase<Month>
	{
		using Base = DateTimeBase<Month>;
		static Base::Derived Get(uint i) { return std::make_shared<Month>(i);}
		constexpr Month(uint v): DateTimeBase<Month>(c(v)){};
		constexpr Month(int v): DateTimeBase<Month>(c((uint)v)){};
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
		
	};
	
	struct Day: DateTimeBase<Day>
	{
		using Base = DateTimeBase<Day>;
		static Base::Derived Get(uint i) { return std::make_shared<Day>(i);}
		static Base::Derived Get(int i) { return std::make_shared<Day>((uint)i);}
		Day(uint v): DateTimeBase<Day>(v){};
	};
	
	
	inline static std::shared_ptr<Month> January = std::make_shared<Month>(1);
	inline static std::shared_ptr<Month> February= std::make_shared<Month>(2);
	inline static std::shared_ptr<Month> March= std::make_shared<Month>(3);
	inline static std::shared_ptr<Month> April= std::make_shared<Month>(4);
	inline static std::shared_ptr<Month> May= std::make_shared<Month>(5);
	inline static std::shared_ptr<Month> June= std::make_shared<Month>(6);
	inline static std::shared_ptr<Month> July= std::make_shared<Month>(7);
	inline static std::shared_ptr<Month> August= std::make_shared<Month>(8);
	inline static std::shared_ptr<Month> September= std::make_shared<Month>(9);
	inline static std::shared_ptr<Month> October= std::make_shared<Month>(10);
	inline static std::shared_ptr<Month> November= std::make_shared<Month>(11);
	inline static std::shared_ptr<Month> December= std::make_shared<Month>(12);
	
	using TP = std::chrono::system_clock::time_point;
	
	class Date;
	
	template<typename ItemT>
	const ItemT& Get(Date const& d)
	{
		return std::get<ItemT>(d.tt);
	};
	

	class Date: public Element
	{
	public:
		using DayType = std::shared_ptr<DateTimes::Day>;
		using MonthType = std::shared_ptr<DateTimes::Month>;
		using YearType = std::shared_ptr<DateTimes::Year>;
		using DateType = std::shared_ptr<DateTimes::Date>;
		using TupleType = std::tuple<DateTimes::Day,DateTimes::Month,DateTimes::Year>;
		
		template<typename ItemT>
		friend const ItemT& Get(Date const& t);
		
		inline static const std::string Identifier = "Date";
						
		Date(std::string s, uint d = 0, uint m = 0, uint y = 0): 
						Element{s},
						tt{Extract(s)}
						{	
							auto dt = Extract(s);
							day = std::make_shared<DateTimes::Day>(std::get<0>(dt));
							month = std::make_shared<DateTimes::Month>(std::get<1>(dt));
							year = std::make_shared<DateTimes::Year>(std::get<2>(dt));					
						}; 
						
		Date(uint d, uint m, uint y): Date(std::string(), d,m, y) { };
		Date(const std::string& s, const TupleType& t): Date(s,  std::get<DateTimes::Day>(t).Value(),  std::get<DateTimes::Month>(t).Value(),  std::get<DateTimes::Year>(t).Value() ) { };
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
		
		MonthType Month() const { return this->month; };
		YearType Year() const { return this->year; };
		DayType Day() const { return this->day; };
		
		bool operator==(DayType m) const{ return *(this->day) == m;};
		bool operator==(DateTimes::Day m) const{ return *(this->day) == m;};
		bool operator==(MonthType m) const{ return *(this->month) == m;};
		bool operator==(DateTimes::Month m) const{ return *(this->month) == m;};
		bool operator==(YearType y) const{ return *(this->year) == y;};
		bool operator==(DateTimes::Year y) const{ return *(this->year)== y;};
		bool operator==(Date date) const{ return *(this->year)== *(date.Year()) && *(this->month)== *(date.Month()) && *(this->day)== *(date.Day());};
		bool operator==(DateType date) const{ return *(this->year)== date->Year() && *(this->month)== date->Month() && *(this->day)== date->Day();};
	private:
		TupleType tt;
		TP tp;
		MonthType month;
		YearType year;
		DayType day;
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
