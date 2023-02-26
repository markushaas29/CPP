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
		constexpr static Entry AdvancePayment{"Abschlag"};
		decltype(auto) Execute()
		{
			auto accGas = Get<AccountType>(iban);
			auto transfers = accGas->GetTransferOf(AdvancePayment);
		}		
		
		std::ostream& Display(std::ostream& os) const 
		{
			return printFilters(os);	
			//return os;
		}
	private:
		IBAN iban;	
		TupleType filters;

		template <size_t I = 0>
		constexpr std::ostream& printFilters(std::ostream& os) const
		{
			if constexpr(I ==std::tuple_size_v<TupleType>)    
				return os;
			else 
			{
				auto item = std::get<I>(filters);
				os<<std::get<I>(filters)<<"\n";
			
				return printFilters<I + 1>(os);
			}
		}
	};
	
	//template<typename A, typename TT>
	//std::ostream& operator<<(std::ostream& out, const Transfer<A,TT>& s){	return s.Display(out);	}
}

