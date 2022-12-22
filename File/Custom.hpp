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
#include "../Common/Configuration.hpp"

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename Config>
	struct Custom: public Account<Custom<Config>>
	{
		using Type = Config;
		using TransferTypes = Configuration::Account::TransferType;
		using TransferType = Transfer<Custom,TransferTypes>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom<Config>>;
		friend class Account<Custom<Config>>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static constexpr const char* Filename = "RaibaKonten2021_1.csv";
		
		static std::ostream& Display(std::ostream& os)	{	return cont.Display(os);	}
	protected:
		template<typename T>
		static std::string Extract(std::string s){	return s;	}		
		static std::string ExtractKey(std::string s){	return s;	}
		
	private:
		Custom()	{ 	};
		~Custom()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Custom& operator=(const Custom&) = delete;
		Custom(const Custom& c) = delete;
	
		template<typename A, typename T>
		friend decltype(auto) Get(const T& t);
		
		inline static Base::AccountContainerType cont = typename Base::AccountContainerType();
		inline static constexpr const char* name = "Custom";
	};
}


#endif
