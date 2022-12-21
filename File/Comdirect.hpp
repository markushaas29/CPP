#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
#include "Account.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
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
	template<unsigned int N = 0>
	struct Comdirect: public Account<Comdirect<N>>
	{
		static constexpr uint Num = N;
		using TransferTypes = Configuration::Account::TransferType;
		using TransferType = Transfer<Comdirect,TransferTypes>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Comdirect<N>>;
		using TextSeparator = T::char_<' '> ;
		friend class Account<Comdirect<N>>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Filename = "Umsaetze_DE832004113306947527";
				
		static std::ostream& Display(std::ostream& os)	{	return cont.Display(os); }
	protected:
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
		
	private:
		Comdirect()	{ 	};
		~Comdirect()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Comdirect& operator=(const Comdirect&) = delete;
		Comdirect(const Comdirect& c) = delete;
	
		template<typename A, typename T>
		friend decltype(auto) Get(const T& t);

		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
		inline static constexpr const char* name = "Comdirect";
	};
}


#endif
