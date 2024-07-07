#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include "../CSV/Element.hpp"
#include "../TypeCounter/TypeCounter.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

template<typename, bool> class PredicateVisitor;

#pragma once

class ICssStyle//: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
//	virtual bool operator==(const ICssStyle& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const ICssStyle& e) const noexcept = 0;
	std::string operator()() const  { return data(); };	
//	virtual std::unique_ptr<ICssStyle> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(Data()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const ICssStyle& e) {	return out<<e.data();}
	virtual std::string data() const  = 0;	
////	virtual constexpr bool operator==(const ICssStyle& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const ICssStyle& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return data.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const CssStyle& e) const{ return Data() == e.Data(); };
////	constexpr std::strong_ordering operator<=>( const CssStyle& e) const noexcept { return Data() <=> e.Data(); }
};
////--------------------------------CssStyle------------------------------------------------

class CssStyle: public ICssStyle
{
public:
 	CssStyle(const std::string& c = "") { };
private:
	virtual std::string data() const  {	return ""; };	
};

