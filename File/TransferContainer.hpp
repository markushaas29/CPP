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
		decltype(auto) GetTransferOf(FilterTypes... filters)
		{ 
			auto result = this->getTransferOf(ContainerType(this->transactions->cbegin(), this->transactions->cend()), filters...);		
			return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		}

		template<typename FilterType>
		decltype(auto) FilterBy(FilterType t) 
		{ 
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
		
		ContainerType getTransferOf(ContainerType&& cont){	return cont; } 

		template <typename FilterT, typename... FilterTypes>
		ContainerType getTransferOf(ContainerType&& cont, FilterT filter, FilterTypes... filters)
		{
			ContainerType result = this->filterBy(std::move(cont), filter);
			return getTransferOf(std::move(result),filters...);
		}
		
	};
}


#endif
