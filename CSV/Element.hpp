#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include <string.h>
#include <map>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#pragma once
class IElement
{
public:
	virtual const std::string Value() const  = 0;	
private:
	virtual std::string check(const std::string& s) = 0;
//	virtual constexpr bool operator==(const IElement& e) const = 0;
//	virtual constexpr std::strong_ordering operator<=>( const IElement& e) const noexcept = 0;
//	virtual constexpr const_iterator Begin() const = 0;
//	constexpr decltype(auto) End() { return data.begin() + size; }
//	constexpr decltype(auto) Size() { return size; }
//
//	constexpr bool operator==(const Element& e) const{ return Value() == e.Value(); };
//	constexpr std::strong_ordering operator<=>( const Element& e) const noexcept { return Value() <=> e.Value(); }
};
//--------------------------------Element------------------------------------------------

class Element: public IElement
{
public:
	inline static const std::string Identifier = "Element";
 	Element(const std::string& s): value{s}, size{s.size()} { };

	const std::string Value() const  {	return value; };	
	//const std::string () const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Size() { return size; }

	bool operator==(const Element& e) const{ return Value() == e.Value(); };
	std::strong_ordering operator<=>( const Element& e) const noexcept { return Value() <=> e.Value(); }
private:
	std::string value;
	std::size_t size;
};

std::ostream& operator<<(std::ostream& out, const Element& e) {	return out<<e.Value();}

template <typename T>
concept ElementConcept = requires(T val) {	val.Value(); };
