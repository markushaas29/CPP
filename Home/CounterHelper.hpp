#include "Counter.hpp"

#pragma once

template<> 
void InputManager<Counter>::Parse(InputIterator begin, InputIterator end)
{
	Instance();
	try
	{
		auto it = begin;
		auto dateLine = String_::Split<T::char_<':'>>(*it);
		auto date = dateLine.cbegin() + 1 != dateLine.cend() ? dateLine[1] : "1.1.2022" ;
		++it;
		for(; it != end; ++it)
		{
			auto vals = String_::Split<T::char_<':'>>(*it);
			auto values = String_::Split<T::char_<';'>>(*(vals.cbegin()+1));
			values.insert(values.begin(), date);
			visitors->at(vals[0])->Add(values.cbegin(),values.cend());
		}
	}
	catch(const std::exception& e)
	{
		Logger::Log<Error>("InputManager<Counter>::Parse ",e.what());
	}
}

struct CurrentValue
{ 
	template<typename It>
	decltype(auto) operator()(It begin, It end) const { return **begin; }
};

struct Difference
{ 
	template<typename It>
	decltype(auto) operator()(It begin, It end) const {	return **(begin) - **(begin + 1); 	}
};

struct DailyConsumption
{ 
	template<typename It>
	decltype(auto) operator()(It begin, It end) const {	
		   std::vector<int> v = {7, 5, 16, 8};
		   return v; 	}
};

class AnnualConsumption
{ 
public:
	AnnualConsumption(DateTimes::Year y): year{y} {};
	template<typename It>
	decltype(auto) operator()(It begin, It end) const 
	{ 
		auto curr = std::find_if(begin,end,[&](const auto t) { return t->Date == year;});
		auto prev = std::find_if(begin,end,[&](const auto t) { return t->Date == lastYear;});
		decltype(**curr - **prev) res;
		if(curr == end)
		{
			Logger::Log<Error>("No value to calculate AnnualConumption for: ", year);
			return res;
		}
		if(prev == end)
		{
			Logger::Log<Error>("No value to calculate AnnualConumption for: ", lastYear);
			return res;
		}
		return **curr - **prev;
	}
private:
	DateTimes::Year year;
	DateTimes::Year lastYear = year.Prev();
};

