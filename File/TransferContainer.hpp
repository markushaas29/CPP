#include <memory>
#include <vector>
#include <tuple>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "TransferComparer.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/BaseContainer.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/Elements.hpp"

#ifndef TRANSFERSCONTAINER_HPP
#define TRANSFERSCONTAINER_HPP

namespace Bank
{
	using DayType = DateTimes::Date::DayType;
	using MonthType = DateTimes::Date::MonthType;
	using YearType = DateTimes::Date::YearType;
	using DateType = DateTimes::Date::Type;
	
	template<typename T>
	class TransferContainer: public BaseContainer<TransferContainer<T>, T>
	{
	public:
		using Type = TransferContainer<T> ;
		using ContainerType = std::vector<T>;
		using Base = BaseContainer<TransferContainer<T>, T>;
		using ContainerPtr = std::unique_ptr<ContainerType>;
		using TypePtr = std::shared_ptr<Type>;
		using Iterator = ContainerType::const_iterator;
		using Comparer = TransferComparer::Date;

		TransferContainer(): Base{std::make_unique<ContainerType>()}{}
		
		template <typename... FilterTypes>
		decltype(auto) GetTransferOf(std::tuple<FilterTypes...> filters)
		{ 
			auto result = this->getTransferOf(ContainerType(this->CBegin(), this->CEnd()), filters);		
			return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		}

		template <typename... FilterTypes>
		decltype(auto) GetTransferOf(FilterTypes... filters)
		{
			auto fsT = std::tuple<FilterTypes...>(filters...);
			auto result = this->getTransferOf(ContainerType(this->CBegin(), this->CEnd()), fsT);		
			return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		}

		template<typename FilterType>
		decltype(auto) FilterBy(FilterType t) 
		{ 
			auto result = filterBy(ContainerType(this->CBegin(), this->CEnd()), t); 
			std::cout<<"SIZE"<<result.size()<<std::endl;
			return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		}
		
		const Iterator Begin() const { return this->CBegin(); }
		const Iterator End() const { return this->CEnd(); }
	private:
		TransferContainer(ContainerPtr c): Base(c){ }

		template<typename FilterType>
		decltype(auto) filterBy(ContainerType&& cont, FilterType t) 
		{ 
			auto result = ContainerType();

			if constexpr (DateTimes::isYMD<FilterType>())
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<DateTimes::Date>(*it) == t; });
			else if constexpr (std::is_same<FilterType,Entry>::value)
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return String_::Contains(GetTransfer<Entry>(*it).Value(), t.Value()); });
			else
				std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<FilterType>(*it) == t; });
			
			if(result.size() == 0)
				Logger::Log<Warning>("No transfer by filtering with ",t);
				
			return result;
		}
		
		ContainerType getTransferOf(ContainerType&& cont){	return cont; } 

		template <size_t I = 0, typename... FilterTypes>
		ContainerType getTransferOf(ContainerType&& cont, std::tuple<FilterTypes...> filters)
		{
			if constexpr (I == std::tuple_size_v<std::tuple<FilterTypes...>>)
				return cont;
			else
			{
				ContainerType result = this->filterBy(std::move(cont), std::get<I>(filters));
				return getTransferOf<I+1>(std::move(result),filters);
			}
		}
		
	};
}


#endif
