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

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<unsigned int N = 0, typename TransferT = std::tuple<IBAN,BIC,Name,DateTimes::Date, Quantity<Sum>, Bank::Direction, Entry>>
	struct Custom: public Account<Custom<N>, TransferT>
	{
		enum{ Num = N };
		using TransferTypes = TransferT;
		using TransferType = Transfer<Custom,TransferTypes>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom, TransferTypes>;
		friend class Account<Custom, TransferTypes>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Name = "Custom";
		inline static constexpr const char* Filename = "RaibaKonten2021_1.csv";
		
		static std::ostream& Display(std::ostream& os)	{	return cont.Display(os);	}
	protected:
		template<typename T>
		static std::string Extract(std::string s){	return s;	}
		
		static std::string ExtractKey(std::string s){	return s;	}
	private:
		template<typename A, typename T>
		friend decltype(auto) Get(const T& t);
		
		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
	};
}


#endif
