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

class ICss//: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
//	virtual bool operator==(const ICss& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const ICss& e) const noexcept = 0;
	std::string operator()() const  { return data(); };	
//	virtual std::unique_ptr<ICss> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(Data()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const ICss& e) {	return out<<e.data();}
	virtual std::string data() const  = 0;	
////	virtual constexpr bool operator==(const ICss& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const ICss& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return data.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const Css& e) const{ return Data() == e.Data(); };
////	constexpr std::strong_ordering operator<=>( const Css& e) const noexcept { return Data() <=> e.Data(); }
};
////--------------------------------Css------------------------------------------------

class Css: public ICss
{
public:
 	Css(const std::string& c = "") { };
private:
	virtual std::string data() const  {	return ""; };	
};

