#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <regex>
#include <memory>
#include "../Unit/Unit.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../Matrix/MatrixFormatter.hpp"
#include "../Functional/Functional.hpp"
#include "../CSV/IHtml.hpp"
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

class PaymeentlItem: IHtml
{
public:
	PaymeentlItem(double d): PaymeentlItem{Quantity<Sum>{d}} {}
	PaymeentlItem(Quantity<Sum> q = Quantity<Sum>{0}): value{q} {}
	auto Value() { return value; }
	std::vector<std::unique_ptr<IElement>> Elements() 
	{
		std::vector<std::unique_ptr<IElement>> v;
		v.push_back(value.Clone());
		return v; 
	} 
	std::vector<std::shared_ptr<IElement>> Elements2() 
	{
		std::vector<std::shared_ptr<IElement>> v;
		v.push_back(value.Clone());
		return v; 
	} 
private:
	Quantity<Sum> value;
	friend std::ostream& operator<<(std::ostream& out, const PaymeentlItem& q)	{	return out<<q.value;	}
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return nullptr; };
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return nullptr; };
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
	PaymentCategory(Quantity<Scalar> p = Quantity<Scalar>{1}, const	std::string& n = "", const Month& m = Month{1}, const Year& y = Year{2025}): proportion{p}, name{n}, month{m}, year{y} 
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
		available = s();

		std::vector<std::vector<std::shared_ptr<IElement>>> v;
		std::for_each(items->cbegin(),items->cend(),[&v](auto& p) { v.push_back(p->Elements2()); });

		auto mf = MatrixFormatter(Init(std::move(v))());

		return mf;
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
	std::string name;
	Month month{1};
	Year year{2025};
	std::unique_ptr<std::vector<std::unique_ptr<PaymeentlItem>>> items;
	friend std::ostream& operator<<(std::ostream& out, const PaymentCategory& q)	
	{
		Quantity<Sum> qs{0};
		auto cs = Constant(qs);
		std::for_each(q.items->cbegin(),q.items->cend(),[&out](auto& v) { out<<*v<<"\n"; });
		return out<<q.calculate();	
	}

	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return nullptr; };
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return nullptr; };
	
};
