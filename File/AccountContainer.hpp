#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "TransferContainer.hpp"
#include "Transfer.hpp"
#include "Direction.hpp"
#include "TransferEndpoint.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Typelist/Typelist.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"


#ifndef ACCOUNTCONTAINER_HPP
#define ACCOUNTCONTAINER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename T, template<typename, typename> class TCont = std::map, template<typename> class Cont = std::vector>
	class AccountContainer
	{
	public:
		using KeyType = typename T::KeyType;
		using DataType = std::shared_ptr<T>;
		using AccountEndpointT =TransferEndpoint<typename T::AccountType>;
		using AccountEndpointType =std::shared_ptr<TransferEndpoint<typename T::AccountType>>;
	public:
		void Insert(KeyType k, DataType t)
		{
			if(!this->Contains(k))
			{
				this->keys.push_back(k);
				this->accounts.insert(std::pair<KeyType, AccountEndpointType>(k,std::make_shared<AccountEndpointT>(t)));
				return;
			}
			
			this->accounts[k]->Add(t);
		}
		
		bool Contains(KeyType k) const { return this->accounts.find(k) != this->accounts.end(); }
		const Cont<KeyType>& Keys() const { return keys; }
		const AccountEndpointType& operator[](KeyType k) 
		{
			if(Contains(k))
				return this->accounts[k]; 
			Logger::Log<Error>("No account found for IBAN: ",k, Logger::Source(""));
			throw std::invalid_argument(k.Value());
		}
				
		std::ostream& Display(std::ostream& out) const
		{
			std::for_each(accounts.cbegin(),accounts.cbegin(),[&out](const auto& a) { a.second->Display(out);; out<<"\n"; });
			return out;
		}
		
		std::ostream& Display(std::ostream& out, const std::vector<std::string>& keys) const
		{
			for(auto k : keys)
			{
				const KeyType key = KeyType(k);
				if(!this->Contains(key))
					continue;
				
				auto p = this->accounts[key];				
				p->Display(out);
				
				out<<std::endl;
			}
			
			return out;
		}
		
		template<typename FilterType>
		decltype(auto) FilterBy(FilterType t) 
		{
			std::cout<<"SIZE"<<t<<accounts.size()<<std::endl;	
			for(auto a : accounts)
				a.second->FilterBy(t);
			
			//~ std::for_each(accounts.cbegin(),accounts.cbegin(),[&](const auto& a) { std::cout<<"SIZE"<<t<<std::endl;a.second->FilterBy(t); }); 
		}
		
		std::ostream& DisplayKeys(std::ostream& out) const
		{
			std::for_each(accounts.cbegin(),accounts.cbegin(),[&out](const auto& a) {out<<a.first<<"\n"; });
			return out;
		}		
	private:
		Cont<KeyType> keys;
		TCont<KeyType, AccountEndpointType> accounts;
	};
}


#endif
