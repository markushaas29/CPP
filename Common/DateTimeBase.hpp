#include <ratio>
#include <chrono>
#include <string>
#include "../CSV/Element.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"
#include "../Validator/Validator.hpp"

#pragma once

template<typename T, typename TChrono, uint Max = 3000, uint Min = 1, typename TChronoVal = uint, typename TValue = TChronoVal>
class DateTimeBase: public Element<T>
{
public:
	using Base = Element<T>;
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
	DateTimeBase(const std::string& s): DateTimeBase(::To<uint>(s)) {  }
	DateTimeBase(uint v): Base(std::to_string(v)), value {RangeValidator<uint,min,max>::Check(v)}, valid{RangeValidator<uint,min,max>::Condition(v)}, chronoValue{(ChronoValueType)v} { }
	Derived operator=(const DateTimeBase& d) 
	{ 
		value = d.value;
		return Derived(value);
	}
	DateTimeBase operator=(const DateTimeBase& d) const { return DateTimeBase(d.value);}
	constexpr bool Valid() const { return valid; }
	explicit constexpr operator uint() const { return value; }
	constexpr operator ChronoType() const { return chronoValue;}
	constexpr auto operator==(const DateTimeBase& d) const  {  return value==d.value; };
	constexpr auto operator<=>(const DateTimeBase& d) const  {  return value<=>d.value; };
	static decltype(auto) Create(std::istream& is, std::ostream& os)
	{
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
	static std::string check(std::string s)
  	{
		auto v = ::To<uint>(s);
		if(v > max || v < min || v == 0)
		{
			Logger::Log<Error>("Value",v," is invalid for",Derived::Identifier);
			s = std::to_string(min);
		}
		return s;
  	}
protected:
	uint value;
private:
	friend std::ostream& operator<<(std::ostream& out, const DateTimeBase& s){	return out<<s.Value();	}
	template<typename T1, typename T2> friend decltype(auto) operator==(const DateTimeBase<T1,T2>& d1, const DateTimeBase<T1,T2>& d2);
	template<typename T1, typename T2> friend decltype(auto) operator<=>(const DateTimeBase<T1,T2>& d1, const DateTimeBase<T1,T2>& d2);
	const ChronoType chronoValue;
	const bool valid;
	constexpr static uint min = Min;
	constexpr static uint max = Max;
};

class Day: public DateTimeBase<Day,std::chrono::day,31>
{
public:
	using Base = DateTimeBase<Day,std::chrono::day,31>;
	static constexpr const char* Identifier = "Day";
	Day(uint v): Base(v){};
	explicit Day(const std::string& v): Base(::To<uint>(v)){};
};
	
class Month: public DateTimeBase<Month,std::chrono::month,12>
{
public:
	using Base = DateTimeBase<Month,std::chrono::month,12>;
	static constexpr const char* Identifier = "Month";
	Month(uint v): Base(v){};
	explicit Month(const std::string& v): Base(::To<uint>(v)){};
};

class Year: public DateTimeBase<Year, std::chrono::year,3000, 1900, int, uint>
{
public:
	using Base = DateTimeBase<Year,std::chrono::year,3000,1900, int, uint>;
	static constexpr const char* Identifier = "Year";
	Year(uint v): Base(v), IsLeap(((std::chrono::year)(*this)).is_leap()){};
	explicit Year(const std::string& v = "1900"): Base(::To<uint>(v)){};
	bool IsLeap;
};
	
bool operator<(const Year& y1, const Year& y2) { return y1.Value() < y2.Value(); };

inline static Month Jan = Month(1);
inline static Month Feb= Month(2);
inline static Month Mar= Month(3);
inline static Month Apr= Month(4);
inline static Month May= Month(5);
inline static Month Jun= Month(6);
inline static Month Jul= Month(7);
inline static Month Aug= Month(8);
inline static Month Sep= Month(9);
inline static Month Oct= Month(10);
inline static Month Nov= Month(11);
inline static Month Dec= Month(12);

template<typename T, typename TC>
decltype(auto) operator==(const DateTimeBase<T,TC>& d1, const DateTimeBase<T,TC>& d2){ return d1.chronoValue == d2.chronoValue;	}

template<typename T, typename TC>
decltype(auto) operator<=>(const DateTimeBase<T,TC>& d1, const DateTimeBase<T,TC>& d2){ return d1.chronoValue <=> d2.chronoValue;	}

