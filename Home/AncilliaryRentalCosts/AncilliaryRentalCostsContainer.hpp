#include <memory>
#include "AncilliaryRentalCostItems.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.h"
#include "../../Quantity/Quantity.h"
#include "../../Calculator/Calculator.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../File/Raiba.hpp"
#include "../../File/Account.hpp"

#ifndef ANCILLIARYRENTALCOSTSCONTAINER_HPP
#define ANCILLIARYRENTALCOSTSCONTAINER_HPP
using YearType = DateTimes::Year;

template<typename S>
class AncilliaryRentalCostsContainer
{
public:
	using Type = AncilliaryRentalCostsContainer;
	using StageT = S;
	using Items = CalculationItems<S>;
	template<typename T>
	using ValueType = std::shared_ptr<T>;
	//~ using Item = UtilitiesStatementItem;
	//~ using ItemsType = std::map<std::string, ValueType<Item>>;
	//~ using StatetementsType = std::map<DateTimes::Year, ValueType<Type>>;
	
	static AncilliaryRentalCostsContainer& Instance()
	{
		static AncilliaryRentalCostsContainer instance;
		return instance;
	}
		
private:
	std::unique_ptr<Items> items = std::unique_ptr<Items>();
	AncilliaryRentalCostsContainer(): year(2000)
	{ 
		Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<std::endl;
		this->Calculate(*items);
	}	
	
	
	~AncilliaryRentalCostsContainer()	{ Logger::Log()<<"Destructor"<<std::endl; }
	template <size_t I = 0, typename... Ts>
	constexpr void Calculate(std::tuple<Ts...> tup) 
	{
		Logger::Log()<<"US: "<<StageT::Number<<std::endl;
		if constexpr(I == sizeof...(Ts))    return;
		else 
		{
			auto i = std::get<I>(*items);
			auto y = DateTimes::Year(2021);
			decltype(i)::Calculate(y);
			std::cout<<decltype(i)::Result(y)<<std::endl;
			
			Calculate<I + 1>(*items);
		}
	}
	
	std::ostream& Display(std::ostream& os) {	return os;	}
	
	
	//~ ValueType<ItemsType> items;
	//~ inline static ValueType<StatetementsType> statements = std::make_shared<StatetementsType>(); 
	
	YearType year;
	
};

//~ template<typename C, typename S = T::char_<'\t'>>
//~ std::ostream& operator<<(std::ostream& strm, const AncilliaryRentalCostsContainer<C,Y>& c){	return c.Display(strm); }


#endif
