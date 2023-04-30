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
			static constexpr bool Check(auto v) { return Type::check(v); } 
			constexpr bool operator ()(auto v) const { return Type::check(v, value); }
		protected:
			constexpr PredicateBase(ValueType v = ValueType{0}): value{v} {}
			constexpr PredicateBase(): value{} {}
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
	class Less: public PredicateBase<Less<T>, T>
	{
		using Base = PredicateBase<Less<T>,T>; 
		friend class PredicateBase<Less<T>,T>;
	public:
		constexpr Less(T v): Base{v}{}
	private:
		static constexpr bool check(auto val, T v) { return static_cast<Base::ValueType>(val) < v; } 
	};

	template<typename Pred, typename It>
    decltype(auto) Total(It begin, It end)
    { 
    	auto acc = Quantity<Sum>{0};              
        if(begin == end)
        {
        	Logger::Log<Error>("No transfers! Cant calculate sum is ", acc);
            return acc;
        }
           
        auto p = Pred{0};
    	acc = std::accumulate(begin, end,acc,[p](auto q, auto t)
				{
					auto s = Bank::GetTransfer<Quantity<Sum>>(*t);
					if(p(s))
						return q + s;
					return q;
				});
		
		return acc;
	}    
}

