#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>
#include "Account.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Home/Parser.hpp"
#include "../String/Parser.hpp"

#ifndef RAIBA_HPP
#define RAIBA_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	template<unsigned int N = 0, typename TransferT = std::tuple<IBAN,BIC,Name,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>>
	struct Raiba: public Account<Raiba<N>, TransferT>
	{
		enum{ Num = N };
		using TransferTypes = TransferT;
		using TransferType = Transfer<Raiba,TransferTypes>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiba, TransferTypes>;
		friend class Account<Raiba, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Name = "Raiba";
		inline static constexpr const char* Filename = "Umsaetze_DE19660623660009232702";
				
		Raiba(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static std::ostream& Display(std::ostream& os)	{	return cont.Display(os);	}
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
		
		static void InterpretTransaction(std::string transaction)
		{
			;
		}
	private:
		template<typename A, typename T>
		friend decltype(auto) Get(const T& t);
	
		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
		inline static String_::Parser parser = String_::Parser();
	};
}


#endif
