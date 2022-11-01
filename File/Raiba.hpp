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
		static constexpr uint Num = N;
		using TransferTypes = TransferT;
		using TransferType = Transfer<Raiba,TransferTypes>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiba<N>, TransferTypes>;
		friend class Account<Raiba<N>, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Filename = "Umsaetze_DE19660623660009232702";
				
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
	public:
		static Raiba& Instance()
		{
			static Raiba instance;
			return instance;
		}
	private:
		Raiba()	{ 	};
		~Raiba()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Raiba& operator=(const Raiba&) = delete;
		Raiba(const Raiba& c) = delete;
	
		template<typename A, typename T>
		friend decltype(auto) Get(const T& t);
		
		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
		inline static String_::Parser parser = String_::Parser();
		inline static constexpr const char* const name = "Raiba";
	};
}


#endif
