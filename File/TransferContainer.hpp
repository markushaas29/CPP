#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/CSV.hpp"

#ifndef TRANSFERSCONTAINER_HPP
#define TRANSFERSCONTAINER_HPP

namespace Bank
{
	using DayType = DateTimes::Date::DayType;
	using MonthType = DateTimes::Date::MonthType;
	using YearType = DateTimes::Date::YearType;
	using DateType = DateTimes::Date::Type;
	
	template<typename T>
	class TransferContainer
	{
	public:
		using Type = TransferContainer<T> ;
		using ContainerType = std::vector<T>;
		using ContainerPtr = std::unique_ptr<ContainerType>;
		using TypePtr = std::shared_ptr<Type>;
		using Iterator = ContainerType::const_iterator;

		TransferContainer(): transactions{std::make_unique<ContainerType>()}{}
		
// 		Type operator[](std::string s) { return Type(ContainerType(this->Begin()+1, this->End()-1)); }

		template<typename FilterType>
		decltype(auto) FilterBy(FilterType t) 
		{ 
			auto result = std::make_unique<ContainerType>();
			if constexpr (std::is_same<FilterType,DateTimes::Year>::value)
				std::copy_if(transactions->begin(), transactions->end(), std::back_inserter(*result), [&t](auto it) { return GetTransfer<DateTimes::Date>(*it) == t; });
			else if constexpr (std::is_same<FilterType,Entry>::value)
				std::copy_if(transactions->begin(), transactions->end(), std::back_inserter(*result), [&t](auto it) { return String_::Contains(GetTransfer<Entry>(*it).Value, t.Value); });
			else
				std::copy_if(transactions->begin(), transactions->end(), std::back_inserter(*result), [&t](auto it) { return GetTransfer<FilterType>(*it) == t; });
				
			return result; 
		}
		
		//~ decltype(auto) GetTransferOf()
		//~ {			
			//~ return 0;
		//~ }

		//~ template <typename FilterT, typename... FilterTypes>
		//~ decltype(auto) GetTransferOf(FilterT var1, FilterTypes... var2)
		//~ {
			//~ return GetTransferOf(var2...);
		//~ }
		
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
		
	private:
		ContainerPtr transactions;
		TransferContainer(ContainerPtr c): transactions(c){ }
	};
}


#endif
