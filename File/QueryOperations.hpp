#include <algorithm>
#include <numeric>
#include "Account.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"

#pragma once

namespace Bank
{	
	template<typename Derived, typename T>
	class PredicateBase
	{
		public:
			using Type = Derived; 
			using ValueType = T; 
			static constexpr bool Check(auto v, ValueType val) { return Type::check(v,val); } 
			constexpr bool operator ()(auto v) const { return Type::check(v, value); }
		protected:
			constexpr PredicateBase(ValueType v): value{v} {}
		private:
			ValueType value;
	};
	
	class Turnover: public PredicateBase<Turnover, double>
	{
		using Base = PredicateBase<Turnover, double>; 
		friend class PredicateBase<Turnover, double>;
	public:
		constexpr Turnover(double v = double{}): Base{v}{}
	private:
		static constexpr bool check(auto val, double v) { return true; } 
	};

	template<typename T = double>
	class Greater: public PredicateBase<Greater<T>, T>
	{
		using Base = PredicateBase<Greater<T>,T>; 
		friend class PredicateBase<Greater<T>,T>;
	public:
		constexpr Greater(T v = T{0}): Base{v}{}
	private:
		static constexpr bool check(auto val, T v) { return static_cast<Base::ValueType>(val) > v; } 
	};

	template<typename T = double>
	class Less: public PredicateBase<Less<T>, T>
	{
		using Base = PredicateBase<Less<T>,T>; 
		friend class PredicateBase<Less<T>,T>;
	public:
		constexpr Less(T v = T{0}): Base{v}{}
	private:
		static constexpr bool check(auto val, T v) { return static_cast<Base::ValueType>(val) < v; } 
	};

	class Expenses: public Less<double>
	{
		using Base = Less<double>;
	public:
		constexpr Expenses(double v = 0.0): Base{0.0}{}
	};

	class Income: public Greater<double>
	{
		using Base = Greater<double>;
	public:
		constexpr Income(double v = 0.0): Base{0.0}{}
	};

	template<typename Pred, typename It>
    decltype(auto) Total(It begin, It end, Pred p = Pred{})
    { 
    	auto acc = Quantity<Sum>{0};              
        if(begin == end)
        {
        	Logger::Log<Error>("No transfers! Cant calculate sum is ", acc);
            return acc;
        }
           
        
    	acc = std::accumulate(begin, end,acc,[p](auto q, auto t)
				{
					auto s = Bank::GetTransfer<Quantity<Sum>>(*t);
					if(p(s))
						return q + s;
					return q;
				});
		
		return acc;
	}

	template<typename A> class QueryResult;
	
	template<typename Pred, typename A>
    decltype(auto) Total(QueryResult<A>& r)
    {
		auto it = r.Items();
      	return Total<Pred>(it->cbegin(),it->cend());
	}
}

