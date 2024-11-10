#include <algorithm>
#include <numeric>
#include <tuple>
#include <memory>
#include <vector>
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"

#pragma once

template<typename A>
class QueryResult
{
public:
	using TransferType = A::TransferType;
	using ContType = std::vector<std::shared_ptr<TransferType>>;
	using ContPtr = std::unique_ptr<ContType>;
	using QuantityType = Quantity<Sum,Pure>;
	QueryResult(ContPtr t) { };
	QuantityType GetSum() { return sum; }

	std::ostream& Display(std::ostream& os) const 
	{ 
		return os; 
	}
	friend 	std::ostream& operator<<(std::ostream& out, const QueryResult<A>& s){	return s.Display(out);	}
private:
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
class Query: public QueryBase<Query,A,Ts...>
{
public:
	using Base = QueryBase<Query,A,Ts...>;
	constexpr Query(Ts... t) :  Base{t...} { };

	decltype(auto) Execute() const
	{
	}
private:
	Query() = delete;
};

