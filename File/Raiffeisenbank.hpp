#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
#include "Account.hpp"
#include "Direction.hpp"
#include "../Common/Analyzer.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Home/Parser.hpp"
#include "../String/Parser.hpp"
#include "../Common/Configuration.hpp"

#ifndef RAIBA_HPP
#define RAIBA_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	
	template<typename Config>
	class Raiffeisenbank: public Account<Raiffeisenbank<Config>>
	{
	public:
		using Type = Config;
		using TransferItems = Configuration::Account::TransferType;
		using TransferType = Transfer<Raiffeisenbank, TransferItems>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiffeisenbank<Type>>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Filename = "Umsaetze_DE19660623660009232702";
				
		static std::ostream& Display(std::ostream& os)	{	return cont->Display(os);	}	

		static Raiffeisenbank& Instance()
		{
			static Raiffeisenbank instance;
			return instance;
		}
	private:
		friend class Account<Raiffeisenbank<Type>>;
		friend class Base::ParseType;
		friend class Analyzer<Base,int>;
		Raiffeisenbank()	{ 	};
		~Raiffeisenbank()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Raiffeisenbank& operator=(const Raiffeisenbank&) = delete;
		Raiffeisenbank(const Raiffeisenbank& c) = delete;
	
		template<typename A, typename T> friend decltype(auto) Get(const T& t);
		template <typename A, typename... FilterTypes> friend decltype(auto) GetTransfersOf(FilterTypes... filters);
				
		inline static std::unique_ptr<typename Base::AccountContainerType> cont = std::make_unique<typename Base::AccountContainerType>();
		inline static String_::Parser parser = String_::Parser();
		inline static constexpr const char* const name = "Raiba";
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			Parser::JSON json;
			auto vals = json.Parse(s);

			auto it = std::find_if (vals.begin(),vals.end(), [](std::pair<std::string, std::string> const& item) { return String_::Contains(item.first,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return (it)->second;
		}
	};
}


#endif
