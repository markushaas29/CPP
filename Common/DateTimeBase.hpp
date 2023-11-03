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
	Derived& operator=(const Derived& d) 
	{ 
		std::cout<<"DAY ASSIGN\n\n\n"<<Derived(d.value)<<std::endl;
		return Derived(d.value);
	}
	DateTimeBase operator=(const DateTimeBase& d) const { return DateTimeBase(d.value);}
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
	uint value;
private:
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
	Day& operator=(const Day& d)
	{ 
		value = d.value;
		return *this;
	}
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

inline constexpr static Month Jan = Month(1);
inline constexpr static Month Feb= Month(2);
inline constexpr static Month Mar= Month(3);
inline constexpr static Month Apr= Month(4);
inline constexpr static Month May= Month(5);
inline constexpr static Month Jun= Month(6);
inline constexpr static Month Jul= Month(7);
inline constexpr static Month Aug= Month(8);
inline constexpr static Month Sep= Month(9);
inline constexpr static Month Oct= Month(10);
inline constexpr static Month Nov= Month(11);
inline constexpr static Month Dec= Month(12);


template<typename T, typename TC>
decltype(auto) operator==(const DateTimeBase<T,TC>& d1, const DateTimeBase<T,TC>& d2){ return d1.chronoValue == d2.chronoValue;	}

template<typename T, typename TC>
decltype(auto) operator<=>(const DateTimeBase<T,TC>& d1, const DateTimeBase<T,TC>& d2){ return d1.chronoValue <=> d2.chronoValue;	}

template<typename T, typename TC>
std::ostream& operator<<(std::ostream& out, const DateTimeBase<T,TC>& s){	return out<<s.Value();	}
