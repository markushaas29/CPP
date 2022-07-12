#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <tuple>
#include <vector>
#include <filesystem>
#include "TransferContainer.hpp"
#include "Transfer.hpp"
#include "Direction.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"


#ifndef ACCOUNTENDPOINT_HPP
#define ACCOUNTENDPOINT_HPP

namespace Bank
{
	template<typename AccountT, typename TupleT,template<typename> class ContT> class TransferEndpoint;
	
	template<typename ItemT, typename AccountT, typename TupleT,template<typename> class Cont>
	const ItemT& GetT(TransferEndpoint<AccountT,TupleT,Cont>const& t)	{ return std::get<ItemT>(t.types);	};
	
	template<typename Account, typename TupleType = std::tuple<Name,IBAN,BIC,Quantity<Sum>>,template<typename> class Cont = std::vector>
	class TransferEndpoint
	{
		using TransferTypes = Account::TupleType;
		using EnpointTypes = TupleType;
		using Type = TransferEndpoint<Account> ;
		using TransferType = Transfer<Account,TransferTypes> ;
		using DataType = std::shared_ptr<TransferType>;
		using ResultContainer = Cont<DataType> ;
		using ContainerType = TransferContainer<DataType>;
		using Iterator = ContainerType::Iterator;
		
		TupleType types;
		std::shared_ptr<ContainerType> transactions = std::make_shared<ContainerType>();
	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using KeyType = Key<std::string>;
		using QuantityType = Quantity<Sum>;
		
		template<typename ItemT, typename AccountT, typename TupleT,template<typename> class C>
		friend const ItemT& GetT(TransferEndpoint<AccountT,TupleT,C>const& t);
		
		TransferEndpoint(std::string ownerKey, std::string i = "IBAN", std::string b = "BIC") : types(ownerKey, i, b) { };
		TransferEndpoint(const DataType t) : types( Bank::Get<Name>(*t), Bank::Get<IBAN>(*t), Bank::Get<BIC>(*t), Bank::Get<Quantity<Sum>>(*t))	{ this->transactions->Add(t); };
		TransferEndpoint():types("ownerKey", "iban", "bic", 0) { };
				
		void Add(DataType t)
		{
			this->transactions->Add(t);
			std::get<QuantityType>(types) = std::get<QuantityType>(types) + Bank::Get<Quantity<Sum>>(*t);
		}
		
		std::ostream& Display(std::ostream& out)
		{
			out<<"Owner: "<<std::get<Name>(types)<<std::endl;
			out<<"\tIBAN: "<<std::get<IBAN>(types)<<"\tBIC: "<<std::get<BIC>(types)<<std::endl;
			auto allDates = All<DateTimes::Date>();
			
			for(const auto y : allDates)
			{
				QuantityType sum = QuantityType{0};
				auto year = DateTimes::Get<DateTimes::Year>(y);
				
				for(auto it = this->transactions->Begin(); it != this->transactions->End(); ++it)
				{
					auto date = Bank::Get<DateTimes::Date>(*(*it));
		
					if(DateTimes::Get<DateTimes::Year>(date) == year)
					{
						auto s = Bank::Get<Quantity<Sum>>(**it);
						out<<"\tDate: "<<date<<"\tSum: "<<std::setprecision(2)<<std::fixed<<s<<std::endl;
						out<<"\t"<<"\t"<<Bank::Get<Entry>(**it)<<std::endl;
						sum = sum + s;
					}
				}
				
				out<<"\n\tTotal "<<year<<"\t:"<<std::setprecision(2)<<std::fixed<<sum<<"\n"<<std::endl;
			}
			
			out<<"\nTotal: "<<std::setprecision(2)<<std::fixed<<std::get<QuantityType>(types)<<std::endl;

			return out<<std::endl;
		}
				
		template<typename T>
		decltype(auto) All()
		{
			auto result = std::vector<T>();
						
			std::for_each(this->transactions->Begin(),this->transactions->End(), [&result](const auto& c) 
			{ 
				auto current = Bank::Get<T>(*c);
				
				bool found = false;
				if(result.size() > 0)
				{
					for(const auto& t : result)
					{
						if(!found && DateTimes::Get<DateTimes::Year>(t) == DateTimes::Get<DateTimes::Year>(current))
							found = true;
					}
				}			
						
				if(!found || result.size() == 0)
					result.push_back(current);
			});

			return result;
		}
		
		ResultContainer GetCause(std::string name = "")
		{
			ResultContainer result;
			for(auto it = this->transactions.Begin(); it != this->transactions.End(); ++it)
			{
				if(String_::Contains(Bank::Get<Entry>(*(*it)).Value, name))
					result.push_back(*it);
			}
			
			return result;
		}		
	};
}


#endif
