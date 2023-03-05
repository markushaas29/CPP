#include <algorithm>
#include <tuple>
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

namespace Bank
{	
	template<typename A, typename... Ts> 
	class AccountQuery 
	{
	public:
		using Type = AccountQuery<A,Ts...> ;
		using TupleType = std::tuple<Ts...>;
		using AccountType = A ;
		using QuantityType = Quantity<Sum,Pure>;
		constexpr AccountQuery(const IBAN& i) : iban{i} { };
		constexpr AccountQuery(const IBAN& i, Ts... t) : iban{i}, filters{TupleType(t...)} { };

		template<typename T>
		constexpr bool operator==(const T t) const { return false; };

		decltype(auto) Execute() const
		{
			auto accGas = Get<AccountType>(iban);
			auto transfers = accGas->GetTransferOf(filters);
			return transfers;
		}		
		
		std::ostream& Display(std::ostream& os) const 
		{ 
			os<<AccountType::Identifier<<"\n";
			os<<iban<<"\n";
			return printFilters(os); 
		}
	private:
		IBAN iban;	
		TupleType filters;
		AccountQuery() = delete;

		template <size_t I = 0>
		constexpr std::ostream& printFilters(std::ostream& os) const
		{
			if constexpr(I ==std::tuple_size_v<TupleType>)    
				return os;
			else 
			{
				os<<std::get<I>(filters)<<"\n";
				return printFilters<I + 1>(os);
			}
		}
	};
	
	template<typename A, typename... Ts>
	std::ostream& operator<<(std::ostream& out, const AccountQuery<A,Ts...>& s){ return s.Display(out);	}
	
	template<typename... Ts> 
	class AccountQueryContainer 
	{
	public:
		using Type = AccountQueryContainer<Ts...> ;
		using TupleType = std::tuple<Ts...>;
		using AccountType = int;
		using QuantityType = Quantity<Sum,Pure>;
		static constexpr size_t QueryCount = std::tuple_size_v<TupleType>;
		constexpr AccountQueryContainer(Ts... t) : queries{TupleType(t...)} { };

		template<typename T>
		constexpr bool operator==(const T t) const {	return false;	};

		auto Execute() const
		{
			auto query = std::get<0>(queries).Execute();
			if constexpr (QueryCount==1)
				return query;
			return execute<1>(std::move(query));
		}		
		
		std::ostream& Display(std::ostream& os) const 	{ return printQueries(os); }
	private:
		TupleType queries;
		AccountQueryContainer() = delete;

		template<size_t I = 0>
		auto execute(auto cont) const
		{
			if constexpr(I == QueryCount)    
				return cont;
			else 
			{
				auto query = std::get<I>(queries).Execute();
				if(query->begin() != query->end())
					cont->insert(cont->end(), query->begin(), query->end());
				else
					Logger::Log<Info>("No Transfers found for categories: ", std::get<I>(queries));
				return execute<I+1>(std::move(cont));
			}
		}		
		template <size_t I = 0>
		constexpr std::ostream& printQueries(std::ostream& os) const
		{
			if constexpr(I == QueryCount)    
				return os;
			else 
			{
				os<<std::get<I>(queries)<<"\n";
				return printQueries<I + 1>(os);
			}
		}
	};
}

