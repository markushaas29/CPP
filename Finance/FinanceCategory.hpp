#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <regex>
#include <memory>
#include "../Unit/Unit.hpp"
#include "../CSV/Element.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Unit/UnitRatio.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../Common/Make/Make.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#pragma once

class FinancialItem
{
public:
	FinancialItem(double d): FinancialItem{Quantity<Sum>{d}} {}
	FinancialItem(Quantity<Sum> q = Quantity<Sum>{0}): value{q} {}
	auto Value() { return value; }
private:
	friend std::ostream& operator<<(std::ostream& out, const FinancialItem& q)	{	return out<<q.value;	}
	Quantity<Sum> value;
	
};

class FinanceCategory
{
public:
    
	FinanceCategory() 
	{
		items = std::make_unique<std::vector<std::unique_ptr<FinancialItem>>>();
		items->push_back(std::make_unique<FinancialItem>(250.0));
	}
//	explicit constexpr FinanceCategory(const T1& v): Base(Calculator::data(v)), value(v * QR::Factor) {	}
//	explicit FinanceCategory(const std::string& s): Base(Calculator::data(Calculator::template stringTo<CurrentType>(s))), value{(Calculator::template stringTo<CurrentType>(s)) * (CurrentType)QR::Factor} { 	}
	
	auto Current() const { return value;}
	
//	constexpr bool Equals(const FinanceCategory<U,QR,T1>& y, double epsilon = 0.001) const 	{	return std::fabs(Current()-y.Current()) <= epsilon; }
//	constexpr decltype(auto) operator<=>(const FinanceCategory<U,QR,T1>& y) const { return value <=> y.value; }
//	constexpr bool operator==(const FinanceCategory<U,QR,T1>& y) const {		return value == y.value; }
//	constexpr operator CurrentType() const { return Current(); }

//	// ----------------------------------------ADD-------------------------------------------------------------
//	constexpr decltype(auto) operator+(const FinanceCategory<U,QR,T1>& y) const { return Type(Current() +y.Current()); }
//	
//	template<typename TQR = QR>
//	constexpr decltype(auto) operator+(const FinanceCategory<U,TQR,T1>& y) const { return Type(Current() + Calculator::transform(y).Current()); }
//	
//	// ----------------------------------------SUB-------------------------------------------------------------
//	constexpr  decltype(auto) operator-(const FinanceCategory<U,QR,T1>& y) const { return Type(Current() - y.Current()); }
//	
//	template<typename TQR = QR>
//	constexpr decltype(auto) operator-(const FinanceCategory<U,TQR,T1>& y) const { return Type(Current() - Calculator::transform(y).Current()); }
private:
	Quantity<Sum> value;
	std::unique_ptr<std::vector<std::unique_ptr<FinancialItem>>> items;
	friend std::ostream& operator<<(std::ostream& out, const FinanceCategory& q)	
	{
		std::for_each(q.items->cbegin(),q.items->cend(),[&out](auto& v) { out<<*v<<"\n"; });
		return out<<q.value;	
	}
};
