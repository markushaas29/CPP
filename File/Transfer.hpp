#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include "Transfers.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Unit/SIPrefix.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Common/DateTimes.hpp"

#ifndef TRANSFER_HPP
#define TRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	template<typename Account, typename TupleT>	class Transfer;
	
	template<typename ItemT, typename Account, typename TupleT>
	const ItemT& Get(Transfer<Account,TupleT>const& t)
	{
		return std::get<ItemT>(t.transferItems);
	};
	
	template<typename Account, typename TupleT>
	class Transfer
	{
	public:
		using CSVSeparator = T::char_<';'> ;
		using Type = Transfer<Account,TupleT> ;
		using TupleType = TupleT;
		using AccountType = Account ;
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		Transfer() = delete;
		Transfer(const TupleType& t) : transferItems(t) { 	};
		Transfer(TupleType&& t) : transferItems{std::move(t)} { 	};

		bool operator==(DateTimes::Day m) const{ return std::get<DateTimes::Date>(transferItems) == m;};
		bool operator==(DateTimes::Month m) const{ return std::get<DateTimes::Date>(transferItems) == m;};
		bool operator==(DateTimes::Year y) const{ return std::get<DateTimes::Date>(transferItems) == y;};
		bool operator==(DateTimes::Date date) const{ return std::get<DateTimes::Date>(transferItems) == date;};
		
		std::ostream& Display(std::ostream& os) const 
		{
			os<<"\tDate: "<<Bank::Get<DateTimes::Date>(*this)<<"\tSum: "<<std::setprecision(2)<<std::fixed<<Bank::Get<Quantity<Sum>>(*this)<<std::endl;
			os<<"\t"<<"\t"<<Bank::Get<Entry>(*this)<<std::endl;
			
			return os;
		}
	private:
		template<typename ItemT, typename A, typename T>
		friend const ItemT& Get(Transfer<A,T>const& t);
		
		TupleType transferItems;
	};
	
	template<typename A, typename TT>
	std::ostream& operator<<(std::ostream& out, const Transfer<A,TT>& s)
	{
		return s.Display(out);		
	}
}


#endif
