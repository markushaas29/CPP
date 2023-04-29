#include <algorithm>
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
			constexpr bool operator ()(auto v) { return Type::check(v, value); }
		protected:
			PredicateBase(ValueType v = ValueType{0}): value{v} {}
		private:
			ValueType value;
	};
	
	template<typename VT = double>
	class Less: public PredicateBase<Less<VT>, VT>
	{
		using Base = PredicateBase<Less<VT>,VT>; 
		friend class PredicateBase<Less<VT>,VT>;
	public:
		Less(VT v): Base{v}{}
	private:
		static constexpr bool check(auto val, VT v) { return static_cast<Base::ValueType>(val) < v; } 
	};
}

