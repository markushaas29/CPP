#include <algorithm>
#include "Account.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"

#pragma once

namespace Bank
{	
	template<typename Derived>
	class PredicateBase
	{
		public:
			using Type = Derived; 
			using ValueType = typename Derived::T; 
			static constexpr bool Check() { return Type::check(); } 
			constexpr bool operator ()() { return Type::check(); }
		protected:
			PredicateBase(ValueType v = ValueType{0}): value{v} {}
			constexpr auto get() { return value; }
		private:
			ValueType value;
	};
	
	template<typename VT = double>
	class Less: PredicateBase<Less<VT>>
	{
		using Base = PredicateBase<Less<VT>>; 
		using T = VT; 
		friend class PredicateBase<Less<VT>>;
		static constexpr bool check(auto val) { return static_cast<Base::ValueType>(val) < Base::get() ; } 
	};
}

