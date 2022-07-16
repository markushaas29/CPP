#include "AncilliaryRentalCostItems.hpp"
#include "../../String/String_.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Unit/Unit.h"
#include "../../Quantity/Quantity.h"
#include "../../Calculator/Calculator.hpp"
#include "../../Common/DateTimes.hpp"

#ifndef ANCILLIARYRENTALCOSTSCONTAINER_HPP
#define ANCILLIARYRENTALCOSTSCONTAINER_HPP
using YearType = DateTimes::Year;

template<typename S,typename Y>
class AncilliaryRentalCostsContainer
{
public:
	using Type = AncilliaryRentalCostsContainer;
	using Stage = S;
	//~ using All = Configuration::All;
	template<typename T>
	using ValueType = std::shared_ptr<T>;
	//~ using Item = UtilitiesStatementItem;
	//~ using ItemsType = std::map<std::string, ValueType<Item>>;
	//~ using StatetementsType = std::map<DateTimes::Year, ValueType<Type>>;
	AncilliaryRentalCostsContainer(): year(2000)
	{ 
		//~ this->items->insert({"TEST",std::make_shared<Item>()});
		Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<std::endl;
		//~ Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<year.Value()<<std::endl;
		//~ auto v = this->items->at("TEST");
	}	
		
private:
	AncilliaryRentalCostsContainer(YearType y): year(y) 
	{ 
		//~ this->items->insert({"TEST",std::make_shared<Item>()});
		Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<y.Value()<<std::endl;
		//~ auto v = this->items->at("TEST");
	}
	
	//~ ~AncilliaryRentalCostsContainer()	{ Logger::Log()<<"Destructor"<<std::endl; }
	void Calculate() 
	{
		Logger::Log()<<"US: "<<Stage::Number<<std::endl;
	}
	
	std::ostream& Display(std::ostream& os) {	return os;	}
	
	
	//~ ValueType<ItemsType> items;
	//~ inline static ValueType<StatetementsType> statements = std::make_shared<StatetementsType>(); 
	
	YearType year;
	
};

template<typename C, typename Y,typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const AncilliaryRentalCostsContainer<C,Y>& c){	return c.Display(strm); }


#endif
