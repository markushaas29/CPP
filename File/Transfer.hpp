#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include "TransferContainer.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#ifndef TRANSFER_HPP
#define TRANSFER_HPP

namespace fs = std::filesystem;

namespace Bank
{	
	template<typename Account, typename TupleT>	class Transfer;
	
	template<typename ItemT, typename Account, typename TupleT>
	const ItemT& GetTransfer(Transfer<Account,TupleT>const& t)	{	return std::get<ItemT>(t.transferItems); };
	
	template<typename Account, typename TupleT>
	class Transfer
	{
	public:
		using CSVSeparator = T::char_<';'> ;
		using Type = Transfer<Account,TupleT> ;
		using TupleType = TupleT;
		using AccountType = Account ;
		using KeyType = typename Account::KeyType;
		using QuantityType = Quantity<Sum,Pure>;
		
		Transfer() = delete;
		Transfer(const TupleType& t) : transferItems(t) { };
		Transfer(TupleType&& t) : transferItems{std::move(t)} {	};

		template<typename T>
		constexpr bool operator==(const T t) const
		{ 
			if constexpr (DateTimes::isYMD<T>())
				return t == std::get<DateTimes::Date>(transferItems); 
			if constexpr (tuple_contains_type<T, TupleType>::value)
				return t == std::get<T>(transferItems);
			return false;
		
		};
		
		template<typename T>
		constexpr explicit operator T() { return std::get<T>(transferItems); } 
		constexpr explicit operator double() { return std::get<QuantityType>(transferItems).Value(); } 
		
		std::ostream& Display(std::ostream& os) const 
		{
			//~ os<<"\tDate: "<<std::get<DateTimes::Date>(transferItems)<<"\tSum: "<<std::get<Direction>(transferItems)<<std::setprecision(2)<<std::fixed<<std::get<QuantityType>(transferItems)<<std::endl;
			os<<"\tDate: "<<std::get<DateTimes::Date>(transferItems)<<"\tSum: "<<std::setprecision(2)<<std::fixed<<std::get<QuantityType>(transferItems)<<std::endl;
			os<<"\t"<<"\t"<<std::get<Entry>(transferItems)<<std::endl;
			
			return os;
		}
	private:
		template<typename ItemT, typename A, typename T>
		friend const ItemT& GetTransfer(Transfer<A,T>const& t);
		
		TupleType transferItems;
	};
	
	template<typename A, typename TT>
	std::ostream& operator<<(std::ostream& out, const Transfer<A,TT>& s){	return s.Display(out);	}
}


#endif
