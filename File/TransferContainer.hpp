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
#include "Transfers.hpp"
#include "Transfer.hpp"
#include "Direction.hpp"
#include "TransferEndpoint.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"


#ifndef TRANSFERCONTAINER_HPP
#define TRANSFERCONTAINER_HPP

namespace fs = std::filesystem;

namespace Bank
{
	//-----------------------------------------------------------------------------------------------TranferContainer-----------------------------------------------------------------------
		
	template<typename T, template<typename, typename> class TCont = std::map, template<typename> class Cont = std::vector>
	class TransferContainer
	{
	public:
		using KeyType = typename T::KeyType;
		using DataType = std::shared_ptr<T>;
		using AccountEndpointType = TransferEndpoint<typename T::AccountType>;
	private:
		Cont<KeyType> keys;
		TCont<KeyType, AccountEndpointType> transfers;
	public:
		void Insert(KeyType k, DataType t)
		{
			if(!this->Contains(k))
			{
				this->keys.push_back(k);
				this->transfers.insert(std::pair<KeyType, AccountEndpointType>(k,AccountEndpointType(t)));
				return;
			}
			
			this->transfers[k].Add(t);
		}
		
		bool Contains(KeyType k) const { return this->transfers.find(k) != this->transfers.end(); }
		const Cont<KeyType>& Keys() { return keys; }
		const AccountEndpointType& operator[](KeyType k) { return this->transfers[k]; }
				
		std::ostream& Display(std::ostream& out) const
		{
			for(auto p : this->transfers)
			{
				p.second.Display(out);
				
				out<<"\nTotal: "<<std::setprecision(2)<<std::fixed<<p.second.GetTotal()<<std::endl;
				
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
				
				auto p = this->transfers[key];				
				p.Display(out);
				
				out<<"\nTotal: "<<std::setprecision(2)<<std::fixed<<p.GetTotal()<<std::endl;
				out<<std::endl;
			}
			
			return out;
		}
		
		std::ostream& DisplayKeys(std::ostream& out) const
		{
			for(auto p : this->transfers)
			{
				out<<p.first<<std::endl;				
			}
			
			return out;
		}		
	};
}


#endif
