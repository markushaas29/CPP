#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <regex>
#include <memory>
#include "../Unit/Unit.hpp"
#include "../Functional/Functional.hpp"
#include "../CSV/Element.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Unit/UnitRatio.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../Common/Make/Make.hpp"
#include "../Common/Date.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#pragma once

class PaymeentlItem
{
public:
	PaymeentlItem(double d): PaymeentlItem{Quantity<Sum>{d}} {}
	PaymeentlItem(Quantity<Sum> q = Quantity<Sum>{0}): value{q} {}
	auto Value() { return value; }
private:
	friend std::ostream& operator<<(std::ostream& out, const PaymeentlItem& q)	{	return out<<q.value;	}
	Quantity<Sum> value;
	
};

class PaymentCategory
{
	auto calculate() const	
	{
		std::vector<Quantity<Sum>> qs;
		std::for_each(items->cbegin(),items->cend(),[&qs](auto& v) { qs.push_back(v->Value()); });
		return Acc{qs};	
	}
public:
	PaymentCategory(Quantity<Scalar> p = Quantity<Scalar>{1}): proportion{p} 
	{
		items = std::make_unique<std::vector<std::unique_ptr<PaymeentlItem>>>();
		items->push_back(std::make_unique<PaymeentlItem>(250.0));
		items->push_back(std::make_unique<PaymeentlItem>(-150.0));
		items->push_back(std::make_unique<PaymeentlItem>(150.0));
		items->push_back(std::make_unique<PaymeentlItem>(-150.0));
	}
	auto operator()(Quantity<Sum> q = Quantity<Sum>{0}) 
	{
		auto s = Mul(Constant{proportion}, Constant{q});
		std::cout<<"C\t"<<s<<std::endl;
		available = s();
	}
//	explicit constexpr PaymentCategory(const T1& v): Base(Calculator::data(v)), value(v * QR::Factor) {	}
//	explicit PaymentCategory(const std::string& s): Base(Calculator::data(Calculator::template stringTo<CurrentType>(s))), value{(Calculator::template stringTo<CurrentType>(s)) * (CurrentType)QR::Factor} { 	}
	
	Quantity<Sum> Current() const { return calculate();}
	auto Available() const { return available;}
	auto Proportion() const { return proportion;}
	
//	constexpr bool Equals(const PaymentCategory<U,QR,T1>& y, double epsilon = 0.001) const 	{	return std::fabs(Current()-y.Current()) <= epsilon; }
//	constexpr decltype(auto) operator<=>(const PaymentCategory<U,QR,T1>& y) const { return value <=> y.value; }
//	constexpr bool operator==(const PaymentCategory<U,QR,T1>& y) const {		return value == y.value; }
//	constexpr operator CurrentType() const { return Current(); }

//	// ----------------------------------------ADD-------------------------------------------------------------
//	constexpr decltype(auto) operator+(const PaymentCategory<U,QR,T1>& y) const { return Type(Current() +y.Current()); }
//	
//	template<typename TQR = QR>
//	constexpr decltype(auto) operator+(const PaymentCategory<U,TQR,T1>& y) const { return Type(Current() + Calculator::transform(y).Current()); }
//	
//	// ----------------------------------------SUB-------------------------------------------------------------
//	constexpr  decltype(auto) operator-(const PaymentCategory<U,QR,T1>& y) const { return Type(Current() - y.Current()); }
//	
//	template<typename TQR = QR>
//	constexpr decltype(auto) operator-(const PaymentCategory<U,TQR,T1>& y) const { return Type(Current() - Calculator::transform(y).Current()); }
private:
	Quantity<Sum> value;
	Quantity<Sum> available;
	Quantity<Scalar> proportion;
	Month month{1};
	std::string name;
	std::unique_ptr<std::vector<std::unique_ptr<PaymeentlItem>>> items;
	friend std::ostream& operator<<(std::ostream& out, const PaymentCategory& q)	
	{
		Quantity<Sum> qs{0};
		auto cs = Constant(qs);
		std::for_each(q.items->cbegin(),q.items->cend(),[&out](auto& v) { out<<*v<<"\n"; });
		return out<<q.calculate();	
	}
};
