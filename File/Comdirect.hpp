#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
#include "Account.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Home/Parser.hpp"
#include "../Common/Configuration.hpp"

#ifndef COMDIRECT_HPP
#define COMDIRECT_HPP

namespace fs = std::filesystem;

namespace Bank
{
	//-----------------------------------------------------------------------------------------------Tranfers-----------------------------------------------------------------------

	template<typename Config>
	class Comdirect: public Account<Comdirect<Config>>
	{
	public:
		using Type = Config;
		using TransferTypes = Configuration::Account::TransferType;
		using TransferType = Transfer<Comdirect,TransferTypes>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Comdirect<Config>>;
		using TextSeparator = T::char_<' '> ;
		friend class Account<Comdirect<Config>>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Filename = "Umsaetze_DE832004113306947527";
				
		static std::ostream& Display(std::ostream& os)	{	return cont.Display(os); }
	private:
		Comdirect()	{ 	};
		~Comdirect()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Comdirect& operator=(const Comdirect&) = delete;
		Comdirect(const Comdirect& c) = delete;
		
		template<typename A, typename T> friend decltype(auto) Get(const T& t);
		template <typename A, typename... FilterTypes>	friend decltype(auto) GetTransfersOf(FilterTypes... filters);

		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
		inline static constexpr const char* name = "Comdirect";
		
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
		
		static std::string ExtractKey(std::string s)
		{
			Parser::JSON json;
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = json.Parse(s);
			
			return vals.begin()->second;
		}
	};
}


#endif
