#include <algorithm>
#include <tuple>
#include <memory>
#include <vector>
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

	template<typename A, typename... Ts> 
	class ItemQuery 
	{
	public:
		using Type = ItemQuery<A,Ts...> ;
		using TupleType = std::tuple<Ts...>;
		using AccountType = A ;
		using ResultType = QueryResult<A> ;
		constexpr ItemQuery(Ts... t) :  filters{TupleType(t...)} { };

		template<typename T>
		constexpr bool operator==(const T t) const { return false; };

		decltype(auto) Execute() const
		{
			auto transfers = GetTransersOf<AccountType>(filters);
			return ResultType{std::move(transfers)};
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
		ItemQuery() = delete;
	
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
	class AccountQuery 
	{
	public:
		using Type = AccountQuery<A,Ts...> ;
		using TupleType = std::tuple<Ts...>;
		using AccountType = A ;
		using ResultType = QueryResult<A> ;
		constexpr AccountQuery(const IBAN& i) : iban{i} { };
		constexpr AccountQuery(const IBAN& i, Ts... t) : iban{i}, filters{TupleType(t...)} { };

		template<typename T>
		constexpr bool operator==(const T t) const { return false; };

		decltype(auto) Execute() const
		{
			auto acc = Get<AccountType>(iban);
			auto transfers = acc->GetTransfersOf(filters);

			return ResultType{std::move(transfers)};
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

