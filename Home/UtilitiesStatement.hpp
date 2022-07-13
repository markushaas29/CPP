#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../Calculator/Calculator.hpp"
#include "../Common/DateTimes.hpp"
#include "Parser.hpp"
#include "CounterConfiguration.hpp"
#include "StageQuantities.hpp"
#include "StageConfiguration.hpp"

#ifndef UTILITIESSTATEMENT_HPP
#define UTILITIESSTATEMENT_HPP
using YearType = DateTimes::Year;

struct UtilitiesStatementItem{};

template<typename ConfigT>
class UtilitiesStatement
{
public:
	using Type = UtilitiesStatement<ConfigT>;
	using Configuration = ConfigT;
	using Stage = Configuration::Stage;
	using All = Configuration::All;
	template<typename T>
	using ValueType = std::shared_ptr<T>;
	using Item = UtilitiesStatementItem;
	using ItemsType = std::map<std::string, ValueType<Item>>;
	using StatetementsType = std::map<DateTimes::Year, ValueType<Type>>;
	
	inline static const char* Name = Configuration::Name;
		
// 	static UtilitiesStatement& Instance()
	static ValueType<Type> Instance(YearType year)
	{
		if(!statements->contains(year))
			statements->insert({year, std::make_shared<Type>(year)});
		return statements->at(year);
	}
	
private:
	UtilitiesStatement(YearType y): year(y), items(std::make_shared<ItemsType>()) 
	{ 
		this->items->insert({"TEST",std::make_shared<Item>()});
		Logger::Log()<<"CTOR: "<<"UtilitiesStatement"<<y.Value()<<std::endl;
		auto v = this->items->at("TEST");
	}
	
	~UtilitiesStatement()	{ Logger::Log()<<"Destructor"<<std::endl; }
	void Calculate() 
	{
		Logger::Log()<<"US: "<<Stage::Number<<std::endl;
		Calculator::Stage<BuildingInsurance>::template Calculate<Stage,All>();
		Calculator::Stage<WasteFees>::template Calculate<Stage,All>();
		Calculator::Stage<ChimneySweeper>::template Calculate<Stage,All>();
		Calculator::Stage<PropertyTax>::template Calculate<Stage,All>();
		Calculator::Stage<Sewage>::template Calculate<Stage,All>();
	}
	
	std::ostream& Display(std::ostream& os) 
	{
		return os;
	}
	
	UtilitiesStatement(): year(2000), items(std::make_shared<ItemsType>()) { 
		this->items->insert({"TEST",std::make_shared<Item>()});
		Logger::Log()<<"CTOR: "<<"UtilitiesStatement"<<year.Value()<<std::endl;
		auto v = this->items->at("TEST");
	}	
	ValueType<ItemsType> items;
	
	inline static ValueType<StatetementsType> statements = std::make_shared<StatetementsType>(); 
	
	YearType year;
	
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const UtilitiesStatement<C> c)
{
	return c.Display(strm);
}


#endif
