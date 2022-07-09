#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <filesystem>
#include "Transfers.hpp"
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

namespace fs = std::filesystem;

namespace Bank
{
	//-----------------------------------------------------------------------------------------------TranferEndpoint-----------------------------------------------------------------------
	
	template<typename Account, template<typename> class Cont = std::vector>
	class TransferEndpoint
	{
		using TransferTypes = Account::TupleType;
		using Type = TransferEndpoint<Account> ;
		using TransferType = Transfer<Account,TransferTypes> ;
		using DataType = std::shared_ptr<TransferType>;
		using ResultContainer = Cont<DataType> ;
		using ContainerType = Transfers<DataType>;
		using Iterator = ContainerType::Iterator;
		
		Name owner;
		IBAN iban;
		BIC bic;
		Quantity<Sum> total;
		Direction direction;
		ContainerType transactions;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using KeyType = Key<std::string>;
		using QunatityType = Quantity<Sum>;
		
		TransferEndpoint(std::string ownerKey, std::string i = "IBAN", std::string b = "BIC") : owner(ownerKey), iban(i), bic(b) { };
		TransferEndpoint(const DataType t) : iban(Bank::Get<IBAN>(*t)),owner(Bank::Get<Name>(*t)), bic(Bank::Get<BIC>(*t)), total(Bank::Get<Quantity<Sum>>(*t)) //,owner(t->GetOwner())
		{ 
			this->transactions = ContainerType();
			this->transactions.Add(t);
		};
		TransferEndpoint():owner("ownerKey"), iban("i"), bic("b"), total(0) { };
		
		const Name& GetOwner() const { return owner; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetTotal() const { return total; }
		const auto& GetDirection() const { return direction.Id(); }		
		
		void Add(DataType t)
		{
			this->transactions.Add(t);
			this->total = this->total + Bank::Get<Quantity<Sum>>(*t);
		}
		
		std::ostream& Display(std::ostream& out)
		{
			out<<"Owner: "<<owner<<std::endl;
			out<<"\tIBAN: "<<iban<<"\tBIC: "<<bic<<std::endl;
			for(auto it = this->transactions.Begin(); it != this->transactions.End(); ++it)
			{
				out<<"\tDate: "<<Bank::Get<DateTimes::Date>(*(*it))<<"\tSum: "<<std::setprecision(2)<<std::fixed<<Bank::Get<Quantity<Sum>>(**it)<<std::endl;
				out<<"\t"<<"\t"<<Bank::Get<Entry>(**it)<<std::endl;
			}
			
			auto result = All<DateTimes::Date>();
			
			for(const auto& t : result)
				out<<t<<std::endl;

			return out<<std::endl;
		}
		
		template<typename T>
		decltype(auto) All()
		{
			auto result = std::vector<T>();
						
			std::for_each(this->transactions.Begin(),this->transactions.End(), [&result](const auto& c) 
			{ 
				auto current = Bank::Get<T>(*c);
								
				bool in = false;
				if(result.size() > 0)
				{
					for(const auto& t : result)
					{
						if(*(t.Year()) == *(current.Year()))
							in = true;
					}
				}			
						
				if(in || result.size() == 0)
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
		
		const ContainerType& Transactions() const { return this->transactions; }
	};
}


#endif
