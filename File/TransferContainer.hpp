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
	
	//~ template<typename ItemT, typename AccountT>
	//~ const ItemT& GetAll(TransferContainer<AccountT> const& tc)
	//~ {
			//~ auto result = std::vector<T>();
						
			//~ std::for_each(tc->Begin(),tc->End(), [&result](const auto& c) 
			//~ { 
				//~ auto current = Bank::Get<T>(*c);
				
				//~ bool found = false;
				//~ if(result.size() > 0)
				//~ {
					//~ for(const auto& t : result)
					//~ {
						//~ if(!found && DateTimes::Get<DateTimes::Year>(t) == DateTimes::Get<DateTimes::Year>(current))
							//~ found = true;
					//~ }
				//~ }			
						
				//~ if(!found || result.size() == 0)
					//~ result.push_back(current);
			//~ });

			//~ return result;
	//~ };
}


#endif
