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
#include "Node.hpp"
#include "RepositoryValue.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Typelist/Typelist.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../CSV/KeyIndex.hpp"
#include "../Common/Configuration.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename A, typename T>
	decltype(auto) Get(const T& t){	return A::cont[t];	};
	
	template<typename Derived>
	class Account
	{
		using TransferItems = Configuration::Account::TransferType;
		inline static constexpr IBAN iban = Derived::Type::Iban;
		inline static constexpr BIC bic = Derived::Type::Bic;
		inline static constexpr uint TransferItemsCount = std::tuple_size_v<TransferItems>;
		
		inline static std::unique_ptr<FS::FileInfo> keyFileInfo = std::make_unique<FS::FileInfo>(std::filesystem::path( std::string(Configuration::Repository::SourcePath) + "/" + std::string(Derived::name) + ".keys"));
	protected:		
		using CSVSeparator = T::char_<';'> ;
		
	public:
		using Type = Account<Derived> ;
		using TransferType = Transfer<Derived, TransferItems>;
		using TupleType = TransferItems;
		using KeyType = IBAN;
		using AccountContainerType = AccountContainer<TransferType>;
		using QuantityType = Quantity<Sum,Pure>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using KeyIndexType = CSV::KeyIndex<KeyType,uint>;
		using KeyIndexContainerType = CSV::KeyIndexContainer<Derived, std::string,uint>;
		using KeyIndexContainerPtrType = std::unique_ptr<KeyIndexContainerType>;
		using TransferItemContainerType = TransferItemContainer<KeyIndexContainerType,TupleType>::ContainerType;
		using ResultValueType = FS::AccountValue<TransferType>;
	
		inline static constexpr const char* const Identifier = Derived::name;

		template<typename Cont>
		void RegisterTo(Cont& cont)
		{
			cont.insert(std::make_pair(iban.Value(),  
			typename Cont::mapped_type(iban.Value(), Identifier, 
			[&](InputIterator begin, InputIterator end){ Parse(begin,end); }, 
			[&](const std::string& s){ return Get(s); }, 
			[&](InputIterator begin, InputIterator end){ return Update(begin,end); })));			
		}	
		
		static Account& Instance()
		{
			static Account instance;
			return instance;
		}
	private:
		//~ decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<TransferType>>(Derived::cont[IBAN("DE97500500000003200029")]->All()); }
		decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<IBAN>>(); }
		
		void Parse(InputIterator begin, InputIterator end)
		{
			if(keyIndices->Empty())
			{
	 			Logger::Log<Error>()<<Derived::name<<" parsing not possible, no keys!"<<std::endl;
				return;
			}
			
			if(begin != end)
			{
				for(auto it = begin;it != end; ++it)
				{
					auto values = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(*it));
					
					if(keyIndices->Check(values))
					{
						uint valueCount = values.size();
						Logger::Log("Updatet Keys from Line:\n\t",*it);
										
						++it;		
						for(;it != end; ++it)
						{
							auto values = String_::Split<CSVSeparator>(String_::Remove<String_::CR>(*it));
							if(valueCount != values.size())
							{
					 			Logger::Log<Error>(Derived::name,": Not enough values to create a transfer in line",*it);
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
		
		void ReadKeyPatterns(InputIterator begin, InputIterator end)
		{
			TransferItemContainerType::Instance().SetKeyIndexContainer(std::move(keyIndices));
			TransferItemContainerType::Instance().Read();
			if(begin==end)
			{
	 			Logger::Log<Error>("ReadKeyPatterns: No Items found for ",Derived::name);
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
			 			Logger::Log<Info>(Derived::name," ReadKeyPatterns: Key found for item: ");
						keyIndices->UpdateKeyPatterns(keyItem, keys);
					}
				}
				catch(std::exception e)
				{
		 			Logger::Log<Error>("ReadKeyPatterns: ",Derived::name,"\t",e.what());
				}
			}
		}
		bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in"); return true; }
	protected:
		inline static KeyIndexContainerPtrType keyIndices = std::make_unique<KeyIndexContainerType>(TransferItemContainerType::Instance().template Create<Derived>());
		
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
			
		Account()
		{
			if(!keyFileInfo->Exists())
				Logger::Log<Error>("No Keys for ", Derived::name, " under ", *keyFileInfo);
				
			auto lines = FS::ReadLines(keyFileInfo->Path());
			ReadKeyPatterns(lines.cbegin(), lines.cend());
		};
		~Account()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Account& operator=(const Account&) = delete;
		Account(const Account& c) = delete;
	};
}

#endif
