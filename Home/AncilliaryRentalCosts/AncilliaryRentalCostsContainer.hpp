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
	using Stage = S;
	//~ using All = Configuration::All;
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
	AncilliaryRentalCostsContainer(): year(2000)
	{ 
		//~ this->items->insert({"TEST",std::make_shared<Item>()});
		Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<std::endl;
		Bank::Raiba<0>::Display(std::cout);
		auto s = Bank::Get<Bank::Raiba<0>>(year);
		std::cout<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<std::endl;
		std::cout<<s<<std::endl;
		DateTimes::Date d;
		s[d];
			//~ auto account = raiba(Sewage::AccountKey);
			//~ account.Display(std::cout);
			//~ auto sewage = account.GetCause(Sewage::CauseString);
		//~ Logger::Log()<<"CTOR: "<<"AncilliaryRentalCostsContainer"<<year.Value()<<std::endl;
		//~ auto v = this->items->at("TEST");
	}	
	
	
	~AncilliaryRentalCostsContainer()	{ Logger::Log()<<"Destructor"<<std::endl; }
	void Calculate() 
	{
		Logger::Log()<<"US: "<<Stage::Number<<std::endl;
	}
	
	std::ostream& Display(std::ostream& os) {	return os;	}
	
	
	//~ ValueType<ItemsType> items;
	//~ inline static ValueType<StatetementsType> statements = std::make_shared<StatetementsType>(); 
	
	YearType year;
	
};

//~ template<typename C, typename S = T::char_<'\t'>>
//~ std::ostream& operator<<(std::ostream& strm, const AncilliaryRentalCostsContainer<C,Y>& c){	return c.Display(strm); }


#endif
