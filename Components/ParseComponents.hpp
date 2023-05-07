#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Forwards.hpp"
#include "../CSV/Elements.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Interfaces.hpp"
#include <map>
#include <memory>

#pragma once
template<typename T> class ParseImpl : public IParse {};

template<typename C>
class ParseImpl<Counter<C>> : public IParse
{
	using T = Counter<C>;
public:
	using Type = ParseImpl<T>;
	inline static constexpr const char* Identifier = "Parse";
	void operator()(Iterator begin, Iterator end){  parse(begin,end); }

	ParseImpl()	{ 	Logger::Log<Info>()<<"ParseImpl initialized "<<std::endl; 	};
	~ParseImpl()	{ }
	ParseImpl& operator=(const ParseImpl&) = delete;
	ParseImpl(const ParseImpl& c) = delete;
private:
	friend T;
	void parse(Iterator begin, Iterator end)
    {
    	for(auto it = (begin + T::Config::Size); it != end; ++it)
        {
        	if(it->size() > 0)
            {
            	auto v = T::Instance().csv->ExtractValues(*it);
				typename T::DataType reading = T::Instance().createReading(v.cbegin(), v.cend());
                T::Instance().addReading(reading);
              }
        }
          
		T::Display(std::cout);     
	}   
};

template<typename D>
class ParseImpl<Account<D>> : public IParse
{
	using T = Account<D>;
public:
	using Type = ParseImpl<T>;
	inline static constexpr const char* Identifier = "Parse";
	void operator()(Iterator begin, Iterator end){  parse(begin,end); }

	ParseImpl()	{ 	Logger::Log<Info>()<<"ParseImpl initialized "<<std::endl; 	};
	~ParseImpl()	{ }
	ParseImpl& operator=(const ParseImpl&) = delete;
	ParseImpl(const ParseImpl& c) = delete;
private:
	friend T;
	
	void parse(Iterator begin, Iterator end)
	{
		if(T::TransferItemContainerType::Instance().Empty())
		{
			Logger::Log<Error>()<<D::name<<" parsing not possible, no keys!"<<std::endl;
			return;
		}
			
		if(begin != end)
		{
			for(auto it = begin;it != end; ++it)
			{
				auto values = String_::Split<T::CSVSeparator>(String_::Remove<String_::CR>(*it));
				if(T::TransferItemContainerType::Instance().Check(values))
				{
					uint valueCount = values.size();
					Logger::Log("Updatet Keys from Line:\n\t",*it);
									
					++it;		
					for(;it != end; ++it)
					{
						auto values = String_::Split<T::CSVSeparator>(String_::Remove<String_::CR>(*it));
						if(valueCount != values.size())
						{
				 			Logger::Log<Error>(D::name,": Not enough values to create a transfer in line",*it);
				 			continue;
						}
						auto tt = T::TransferItemContainerType::Instance().template CreateTransfer<typename T::TransferType>(values.cbegin(),values.end());
						//D::cont.Insert(Bank::GetTransfer<T::KeyType>(*tt).Value(), tt);
					}
						
					return;
				}
			}
		}

		return;
	}
};
