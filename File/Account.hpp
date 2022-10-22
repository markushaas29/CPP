#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <tuple>
#include <exception>
#include "TransferItemContainer.hpp"
#include "RepositoryValue.hpp"
#include "JSONParser.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../CSV/KeyIndex.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename A, typename T>
	decltype(auto) Get(const T& t){	return A::cont[t];	};
	
	template<typename Derived, typename TransferT>
	class Account
	{
	protected:		
		using CSVSeparator = T::char_<';'> ;
		Key<std::string> owner;
		IBAN iban;
		BIC bic;
		
		inline static const std::string KeysFilename = std::string(Derived::Name) + ".keys";
		inline static constexpr uint TransferItemsCount = std::tuple_size_v<TransferT>;
	public:
		using Type = Account<Derived,TransferT> ;
		using TransferType = Transfer<Derived, TransferT>;
		using TupleType = TransferT;
		using KeyType = IBAN;
		using AccountContainerType = AccountContainer<TransferType>;
		using QuantityType = Quantity<Sum>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using KeyIndexType = CSV::KeyIndex<KeyType,uint>;
		using KeyIndexContainerType = CSV::KeyIndexContainer<Derived, std::string,uint>;
		using KeyIndexContainerPtrType = std::shared_ptr<KeyIndexContainerType>;
		using TransferItemContainerType = TransferItemContainer<KeyIndexContainerType,TupleType>::ContainerType;
		inline static constexpr const char* Identifier = Derived::Name;
		
		static void Parse(InputIterator begin, InputIterator end)
		{
			if(keyIndices->Empty())
			{
	 			Logger::Log<Error>()<<Derived::Name<<" parsing not possible, no keys!"<<std::endl;
				return;
			}
			
			if(begin != end)
			{
				for(auto it = begin;it != end; ++it)
				{
					auto values = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(*it));
					
					if(keyIndices->UpdateKeys(values))
					{
						uint valueCount = values.size();
						Logger::Log("Updatet Keys from Line:\n\t",*it);
										
						++it;		
						for(;it != end; ++it)
						{
							auto values = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(*it));
							if(valueCount != values.size())
							{
					 			Logger::Log<Error>(Derived::Name,": Not enough values to create a transfer in line",*it);
					 			continue;
							}
							auto tt = TransferItemContainerType::Instance().template CreateTransfer<TransferType>(values.cbegin(),values.end());
							Derived::cont.Insert(Bank::GetTransfer<KeyType>(*tt).Value(), tt);
						}
						
						return;
					}
				}
			}

			return;
		}
		
		//~ static decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<TransferType>>(Derived::cont[IBAN()].All); }
		static decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<IBAN>>(); }
		static bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in",Derived::Name); return true; }
		
		static void ReadKeyPatterns(InputIterator begin, InputIterator end)
		{
			TransferItemContainerType::Instance().setKeyIndexContainer(keyIndices);
			TransferItemContainerType::Instance().Read();
			if(begin==end)
			{
	 			Logger::Log<Error>("ReadKeyPatterns: No Items found for ",Derived::Name);
	 			// Todo weiter Fehlerbehandlung für weiter Bearbeitung
	 			return;
			}
						
			for(auto it = begin; it != end; ++it)
			{
				try
				{
					auto values = String_::Split<T::char_<':'>>(*it);
					auto keyItem = *values.cbegin();
					auto keys = String_::Split<T::char_<';'>>(String_::Remove<String_::CR>(*(values.cbegin()+1)));
					
		 			if(keys.cbegin() != keys.cend())
		 			{
			 			Logger::Log<Info>(Derived::Name," ReadKeyPatterns: Key found for item: ");
						keyIndices->UpdateKeyPatterns(Key(keyItem), keys);
					}
				}
				catch(std::exception e)
				{
		 			Logger::Log<Error>("ReadKeyPatterns: ",Derived::Name,"\t",e.what());
				}
			}
		}
				
		template<typename Cont>
		static void RegisterTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  typename Cont::mapped_type(Derived::Filename, Identifier, &Type::Parse, &Type::Get, &Type::Update)));
			cont.insert(std::make_pair(Type::KeysFilename, typename Cont::mapped_type(Type::KeysFilename, "AccountKeys",&Type::ReadKeyPatterns, &Type::Get)));
		}	
		
	protected:
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		inline static KeyIndexContainerPtrType keyIndices = std::make_shared<KeyIndexContainerType>(TransferItemContainerType::Instance().template Create<Derived>());
		
		static std::string GetNumericValue(std::string s)
		{
			std::string result;
			for (unsigned int i = 0; i < s.size(); ++i)
			{
				if(std::isdigit(s.at(i)))
					result += s.at(i);
			}
			
			if(result.size() > 2)
				result.insert(result.size()-2, ".");
			
			return result;
		}
	};
}

#endif
