#include <algorithm>
#include <tuple>
#include "../Logger/Logger.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../Common/TupleHelper.hpp"

#pragma

namespace Bank
{	
	template<typename A, typename... Ts> 
	class AccountQuery 
	{
	public:
		using Type = AccountQuery<A,Ts...> ;
		using TupleType = std::tuple<Ts...>;
		//using FilterTypes = Ts... ;
		using AccountType = A ;
		using QuantityType = Quantity<Sum,Pure>;
		
		AccountQuery() = delete;
		constexpr AccountQuery(const IBAN& i) : iban{i} { };
		constexpr AccountQuery(const IBAN& i, Ts... t) : iban{i}, filters{TupleType(t...)} { };
		//AccountQuery(TupleType&& t) : transferItems{std::move(t)} {	};

		template<typename T>
		constexpr bool operator==(const T t) const
		{ 
			return false;
		
		};
		
		
		std::ostream& Display(std::ostream& os) const 
		{
			return os;
		}
	private:
		IBAN iban;	
		TupleType filters;
	};
	
	//template<typename A, typename TT>
	//std::ostream& operator<<(std::ostream& out, const Transfer<A,TT>& s){	return s.Display(out);	}
}

