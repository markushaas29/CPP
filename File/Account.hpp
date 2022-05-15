#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "JSONParser.hpp"
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
	template<typename Derived>
	class Account
	{
	protected:		
		using CSVSeparator = T::char_<';'> ;
		Key<std::string> owner;
		IBAN iban;
		BIC bic;
		
		inline static const std::string  KeysFilename = Derived::Name + ".keys";
	public:
		using Type = Account<Derived> ;
		using InTransfer = AccountTransfer<Derived,Transfer<In>>;
		using OutTransfer = AccountTransfer<Derived, Transfer<Out>>;
		using KeyType = Key<std::string>;
		using ParseContIn = TransferContainer<AccountTransfer<Derived,Transfer<In>>>;
		using ParseContOut = TransferContainer<AccountTransfer<Derived,Transfer<Out>>>;
		using QunatityType = Quantity<Sum>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using KeyMapType = std::map<const char*, uint>;
		using KeyMapPtrType = std::unique_ptr<KeyMapType>;
		using KeyIndexType = CSV::KeyIndex<KeyType,uint>;
		using KeyIndexContainerType = CSV::KeyIndexContainer<std::string,uint>;
		using KeyIndexContainerPtrType = std::unique_ptr<KeyIndexContainerType>;
		
		static void Parse(InputIterator begin, InputIterator end)
		{
			if(begin != end)
			{
				uint ctr = 0;
							
				auto lines = std::vector<std::string>(begin + Derived::HeaderLength, end - Derived::TrailerLength);
				keyIndices->UpdateKeys(lines);
				
				for(auto line : lines)
				{
					auto values = String_::Split<CSVSeparator>(line);
					
					if (values.size() < MaxIdx)
						continue;
										
					//~ Derived::ProcessValues(values.cbegin(), values.cend());					
				}
			}

			return;
		}
		static void CreacteKeys(InputIterator begin, InputIterator end)
		{
				Logger::Log()<<"CREATEKEYS"<<std::endl;
			for(auto it = begin; it != end; ++it)
			{
				auto values = String_::Split<T::char_<':'>>(*it);
				auto keyItem = *values.cbegin();
				auto keys = String_::Split<T::char_<':'>>(*(values.cbegin() + 1));
				Logger::Log()<<keyItem<<"\t"<<keys[0]<<std::endl;
			}
		}
				
		template<typename Cont>
		static void RegisterTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  &Type::Parse));
			cont.insert(std::make_pair(Type::KeysFilename,  &Type::CreacteKeys));
		}	
		
	protected:
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		
		static constexpr unsigned int Indices[4] = {Derived::OwnerIdx, Derived::DateIdx, Derived::TranactionIdx, Derived::QuantityIdx};
		inline static KeyMapPtrType keyMap = std::make_unique<KeyMapType>();
		inline static KeyIndexContainerPtrType keyIndices = std::make_unique<KeyIndexContainerType>();
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		
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
		
		static void InsertInContainer(std::string key, std::string transaction, double sum, std::string date, std::string iban, std::string bic, char transferSign, std::string cause = "")
		{
// 			Logger::Log()<<date<<"\t"<<sum<<"\t"<<iban<<std::endl;
			if(Derived::IsOutTransfer(transferSign))
				Derived::OutCont.Insert(key, std::make_shared<OutTransfer>(key,transaction,sum, date, iban, bic, cause));
			else
				Derived::InCont.Insert(key,  std::make_shared<InTransfer>(key,transaction,sum, date, iban, bic, cause));
			
		}
	};
}

template<typename A, typename Direction>
struct Get{};

template<typename A>
struct Get<A, Bank::Out>
{
	Bank::AccountEndpoint<A, Bank::Transfer<Bank::Out>> operator()(typename A::KeyType k)
	{
		return A::OutCont[k];
	}
};

template<typename A>
struct Get<A, Bank::In>
{
	Bank::AccountEndpoint<A, Bank::Transfer<Bank::In>> operator()(typename A::KeyType k)
	{
		return A::InCont[k];
	}
};

template<typename A, typename Direction = Bank::Out>
struct Transfers
{
	const typename A::ParseContOut& operator()()
	{
		return A::OutCont;
	}
};

template<typename A>
struct Transfers<A, Bank::In>
{
	const typename A::ParseContIn& operator()()
	{
		return A::InCont;
	}
};

#endif
