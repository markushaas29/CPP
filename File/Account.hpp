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
#include "../Components/ParseComponents.hpp"
#include "../Components/Interfaces.hpp"

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename A, typename T>
	decltype(auto) Get(const T& t){	return A::cont[t];	};
	
	template <typename A, typename... FilterTypes>
	decltype(auto) GetTransfersOf(FilterTypes... filters){ return A::cont.GetTransfersOf(filters...);	};
	
	template<typename Derived>
	class Account
	{
		using TransferItems = Configuration::Account::TransferType;
		template<typename T, typename A> friend class Analyzer;
	public:
		using Type = Account<Derived> ;
		using TransferType = Transfer<Derived, TransferItems>;
		using TupleType = TransferItems;
		using KeyType = IBAN;
		using AccountContainerType = AccountContainer<TransferType>;
		using QuantityType = Quantity<Sum,Pure>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using ParseType = ParseImpl<Derived>;
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
	
	protected:
		using CSVSeparator = T::char_<';'> ;
		
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
	private:
		inline static constexpr IBAN iban = Derived::Type::Iban;
		inline static constexpr BIC bic = Derived::Type::Bic;
		inline static constexpr uint TransferItemsCount = std::tuple_size_v<TransferItems>;
		inline static std::unique_ptr<FS::FileInfo> keyFileInfo = std::make_unique<FS::FileInfo>(std::filesystem::path( std::string(Configuration::Repository::SourcePath) + "/" + std::string(Derived::name) + ".keys"));
		inline static std::unique_ptr<IParse> iParse = std::make_unique<ParseType>();
		//~ decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<TransferType>>(Derived::cont[IBAN("DE97500500000003200029")]->All()); }
		decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<IBAN>>(); }
		
		void Parse(InputIterator begin, InputIterator end) { (*iParse)(begin,end); }
		void ReadKeyPatterns(InputIterator begin, InputIterator end){ TransferItemContainerType::Instance().UpdateKeyIndices(begin,end); }
		bool Update(InputIterator begin, InputIterator end) { Logger::Log("Update in"); return true; }
	};
}

#endif
