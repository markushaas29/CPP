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
#include "../Typelist/Typelist.h"
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
	private:
		Cont<KeyType> keys;
		TCont<KeyType, AccountEndpointType> accounts;
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
		const Cont<KeyType>& Keys() { return keys; }
		const AccountEndpointType& operator[](KeyType k) 
		{
			if(!Contains(k))
				Logger::Log<Error>("No account found for IBAN: ",k);
			return this->accounts[k]; 
		}
				
		std::ostream& Display(std::ostream& out) const
		{
			for(auto it = accounts.cbegin(); it != accounts.cend(); ++it)
			{
				it->second->Display(out);
				out<<std::endl;
			}
			
			return out;
		}
		
		std::ostream& Display(std::ostream& out, const std::vector<std::string>& keys)
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
		
		std::ostream& DisplayKeys(std::ostream& out) const
		{
			for(auto p : this->accounts)
				out<<p.first<<std::endl;				
			
			return out;
		}		
	};
}


#endif
