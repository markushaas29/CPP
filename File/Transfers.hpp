#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/CSV.hpp"

#ifndef ACCOUNTTRANSFERSCONTAINER_HPP
#define ACCOUNTTRANSFERSCONTAINER_HPP

namespace Bank
{
	using DayType = DateTimes::Date::DayType;
	using MonthType = DateTimes::Date::MonthType;
	using YearType = DateTimes::Date::YearType;
	using DateType = DateTimes::Date::Type;
	
	template<typename T>
	class Transfers
	{
	public:
		using Type = Transfers<T> ;
		using ContainerType = std::vector<T>;
		using ContainerPtr = std::shared_ptr<ContainerType>;
		using TypePtr = std::shared_ptr<Type>;
		using Iterator = ContainerType::const_iterator;

		Transfers(): transactions{std::make_shared<ContainerType>()}{}
		
// 		Type operator[](std::string s) { return Type(ContainerType(this->Begin()+1, this->End()-1)); }
		
		template<typename KeyT>
		TypePtr operator[](KeyT t) 
		{ 
			auto result = std::make_shared<Type>();
			std::for_each(this->Begin(), this->End(),[&](auto i){ if(*i == t) result->Add(i);});
			
			return result;  
		}
		
		const Iterator Begin() const { return this->transactions->cbegin(); }
		const Iterator End() const { return this->transactions->cend(); }
		const size_t Size() const { return this->transactions->size(); }
		void Add(T t){ this->transactions->push_back(t); }
		void Display()
		{ 
			
		}
	private:
		ContainerPtr transactions;
		Transfers(ContainerPtr c): transactions(c){ }
	};
}


#endif
