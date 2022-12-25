#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include <chrono>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/Elements.hpp"

#pragma once
	
	
template<typename T>
class ReadingContainer
{
public:
	using Type = ReadingContainer<T> ;
	using ContainerType = std::vector<T>;
	using ContainerPtr = std::unique_ptr<ContainerType>;
	using TypePtr = T;
	using Iterator = ContainerType::const_iterator;

	ReadingContainer(): readings{std::make_unique<ContainerType>()}{}
		
	//~ template <typename... FilterTypes>
	//~ decltype(auto) GetTransferOf(FilterTypes... filters)
	//~ { 
			//~ auto result = this->getTransferOf(ContainerType(this->readings->cbegin(), this->readings->cend()), filters...);		
			//~ return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		//~ }

		//~ template<typename FilterType>
		//~ decltype(auto) FilterBy(FilterType t) 
		//~ { 
			//~ auto result = filterBy(ContainerType(this->readings->cbegin(), this->readings->cend()), t); 
			//~ return std::make_unique<ContainerType>(result.cbegin(), result.cend());
		//~ }
		
	const Iterator Begin() const { return this->readings->begin(); }
	const Iterator End() const { return this->readings->end(); }
	const Iterator CBegin() const { return this->readings->cbegin(); }
	const Iterator CEnd() const { return this->readings->cend(); }
	const size_t Size() const { return this->readings->size(); }
	void Add(T t){ this->readings->push_back(t); }
		
private:
	ContainerPtr readings;
	ReadingContainer(ContainerPtr c): readings(c){ }

		//~ template<typename FilterType>
		//~ decltype(auto) filterBy(ContainerType&& cont, FilterType t) 
		//~ { 
			//~ auto result = ContainerType();
			//~ if constexpr (std::is_same<FilterType,DateTimes::Year>::value)
				//~ std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<DateTimes::Date>(*it) == t; });
			//~ else if constexpr (std::is_same<FilterType,Entry>::value)
				//~ std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return String_::Contains(GetTransfer<Entry>(*it).Value(), t.Value()); });
			//~ else
				//~ std::copy_if(cont.begin(), cont.end(), std::back_inserter(result), [&t](auto it) { return GetTransfer<FilterType>(*it) == t; });
			
			//~ if(result.size() == 0)
				//~ Logger::Log<Warning>("No transfer by filtering with ",t);
				
			//~ return result; 
		//~ }
		
		//~ ContainerType getTransferOf(ContainerType&& cont){	return cont; } 

		//~ template <typename FilterT, typename... FilterTypes>
		//~ ContainerType getTransferOf(ContainerType&& cont, FilterT filter, FilterTypes... filters)
		//~ {
			//~ ContainerType result = this->filterBy(std::move(cont), filter);
			//~ return getTransferOf(std::move(result),filters...);
		//~ }
		
};

