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
		
		template <typename... FilterTypes>
		decltype(auto) GetTransferOf(FilterTypes... filters){ return this->getTransferOf(std::make_unique<ContainerType>(), filters...);		}
		
		//~ template<typename FilterT>
		//~ TypePtr operator[](FilterT t) 
		//~ { 
			//~ auto r = this->filterBy(std::make_unique<ContainerType>(), t); 
			//~ return r; 
		//~ }
		
		template<typename FilterType>
		decltype(auto) FilterBy(FilterType t) 
		{ 
			auto container = ContainerType(this->transactions->cbegin(), this->transactions->cend());
			auto result = this->filterBy(ContainerType(this->transactions->cbegin(), this->transactions->cend()), t); 
			return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		}
		
		const Iterator Begin() const { return this->transactions->cbegin(); }
		const Iterator End() const { return this->transactions->cend(); }
		const size_t Size() const { return this->transactions->size(); }
		void Add(T t){ this->transactions->push_back(t); }
		
	private:
		ContainerPtr transactions;
		TransferContainer(ContainerPtr c): transactions(c){ }

		template<typename FilterType>
		decltype(auto) filterBy(ContainerType&& cont, FilterType t) 
		{ 
			auto result = ContainerType();
			if constexpr (std::is_same<FilterType,DateTimes::Year>::value)
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<DateTimes::Date>(*it) == t; });
			else if constexpr (std::is_same<FilterType,Entry>::value)
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return String_::Contains(GetTransfer<Entry>(*it).Value, t.Value); });
			else
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<FilterType>(*it) == t; });
				
			return result; 
		}
		
		decltype(auto) getTransferOf(ContainerPtr ptr){	return ptr; } 

		template <typename FilterT, typename... FilterTypes>
		decltype(auto) getTransferOf(ContainerPtr ptr, FilterT filter, FilterTypes... filters)
		{
			Logger::Log()<<"GetTransferOf.."<<std::endl;
			return GetTransferOf(filters...);
		}
		
	};
}


#endif
