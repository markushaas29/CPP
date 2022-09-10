#include <memory>
#include <fstream>
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
		auto fs = std::make_unique<std::ofstream>(std::string(StageT::Name)+ ".txt");
		fs = this->Calculate(std::move(fs),*items);
		fs->close();
	}	
	
	
	~AncilliaryRentalCostsContainer()	{  }
	template <size_t I = 0, typename... Ts>
	constexpr std::unique_ptr<std::ofstream> Calculate(std::unique_ptr<std::ofstream> fs, std::tuple<Ts...> tup) 
	{
		if constexpr(I == sizeof...(Ts))    
			return fs;
		else 
		{
			auto i = std::get<I>(*items);
			auto y = DateTimes::Year(2021);
			decltype(i)::Calculate(y);
			(*fs)<<decltype(i)::Result(y)<<std::endl;
			
			return Calculate<I + 1>(std::move(fs), *items);
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
