#include <algorithm>
#include <tuple>
#include <memory>
#include <vector>
#include "Account.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

namespace Bank
{	

         template<typename It>
         static decltype(auto) TotalSum(It begin, It end) 
         { 
                 auto acc = Quantity<Sum>{0};
                 if(begin == end)
                 {
                         Logger::Log<Error>("No transfers! Cant calculate sum is ", acc);
                         return acc;
                 }
                  
                 acc = Bank::GetTransfer<Quantity<Sum>>(**(begin));
                 std::for_each(begin+1, end, [&](const auto t){ acc = acc + Bank::GetTransfer<Quantity<Sum>>(*t); });
                                  
                 return acc;
      	}                                    
             
	
	template<typename A>
	class QueryResult
	{
	public:
		using TransferType = A::TransferType;
		using ContType = std::vector<std::shared_ptr<TransferType>>;
		using ContPtr = std::unique_ptr<ContType>;
		using QuantityType = Quantity<Sum,Pure>;
		QueryResult(ContPtr t): transfers{std::move(t)}, sum{TotalSum(transfers->cbegin(), transfers->cend())} { };
		QuantityType GetSum() { return sum; }
		decltype(auto) Items() { return std::make_unique<ContType>(transfers->cbegin(), transfers->cend()); }

		std::ostream& Display(std::ostream& os) const 
		{ 
			for(auto transfer : *transfers)
				os<<*transfer<<"\n";
			return os; 
		}
		friend 	std::ostream& operator<<(std::ostream& out, const QueryResult<A>& s){	return s.Display(out);	}
	private:
		ContPtr transfers;
		QuantityType sum;
	};

	template<template<typename, typename...> class D,typename A, typename... Ts> 
	class QueryBase 
	{
	public:
		using Type = D<A,Ts...>;
		using TupleType = std::tuple<Ts...>;
		using AccountType = A ;
		using ResultType = QueryResult<A> ;

		template<typename T>
		constexpr bool operator==(const T t) const { return false; };

		decltype(auto) Execute() const
		{
			auto transfers = Bank::GetTransfersOf<AccountType>(filters);
			return ResultType{std::move(transfers)};
		}

		std::ostream& Display(std::ostream& os) const 
		{ 
			os<<AccountType::Identifier<<"\n";
			return printFilters(os); 
		}
	protected:
		constexpr QueryBase(Ts... t) :  filters{TupleType(t...)} { };
		constexpr TupleType getFilters() const { return filters; };
	private:
		TupleType filters;
		QueryBase() = delete;
	
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
	class ItemQuery: public QueryBase<ItemQuery,A,Ts...>
	{
	public:
		using Base = QueryBase<ItemQuery,A,Ts...>;
		constexpr ItemQuery(Ts... t) :  Base{t...} { };

		decltype(auto) Execute() const
		{
			auto transfers = Bank::GetTransfersOf<typename Base::AccountType>(Base::getFilters());
			return typename Base::ResultType{std::move(transfers)};
		}
	private:
		ItemQuery() = delete;
	};

	template<typename A, typename... Ts> 
	class AccountQuery: public QueryBase<ItemQuery,A,Ts...>
	{
	public:
		using Base = QueryBase<ItemQuery,A,Ts...>;
		constexpr AccountQuery(const IBAN& i) : iban{i} { };
		constexpr AccountQuery(const IBAN& i, Ts... t) : iban{i}, Base{t...} { };

		decltype(auto) Execute() const
		{
			auto acc = Get<typename Base::AccountType>(iban);
			auto transfers = acc->GetTransfersOf(Base::getFilters());

			return typename Base::ResultType{std::move(transfers)};
		}

		std::ostream& Display(std::ostream& os) const 
		{ 
			os<<iban<<"\n";
			return Base::Display(os); 
		}
	private:
		IBAN iban;	
		AccountQuery() = delete;
	};
	
	template<typename A, typename... Ts>
	std::ostream& operator<<(std::ostream& out, const AccountQuery<A,Ts...>& s){ return s.Display(out);	}
	
	template<typename... Ts> 
	class AccountQueryContainer 
	{
	public:
		using Type = AccountQueryContainer<Ts...> ;
		using TupleType = std::tuple<Ts...>;
		using AccountType = std::tuple_element<0,TupleType>::type::AccountType;
		using ResultType = QueryResult<AccountType> ;
		using QuantityType = Quantity<Sum,Pure>;
		static constexpr size_t QueryCount = std::tuple_size_v<TupleType>;
		constexpr AccountQueryContainer(Ts... t) : queries{TupleType(t...)} { };

		template<typename T>
		constexpr bool operator==(const T t) const {	return false;	};

		auto Execute() const
		{
			auto query = std::get<0>(queries).Execute();
			if constexpr (QueryCount==1)
				return ResultType(query.Items());
			auto items =  execute<1>(std::move(query.Items()));
			return ResultType(std::move(items));
		}		
		
		std::ostream& Display(std::ostream& os) const 	{ return printQueries(os); }
	private:
		TupleType queries;
		Quantity<Sum> sum{0};
		AccountQueryContainer() = delete;

		template<size_t I = 0>
		auto execute(auto cont) const
		{
			if constexpr(I == QueryCount)    
				return cont;
			else 
			{
				auto queryResult = std::get<I>(queries).Execute();
				auto query = queryResult.Items();
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

