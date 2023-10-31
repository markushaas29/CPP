#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
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
	virtual const std::string Get() const  = 0;	
	virtual std::unique_ptr<IElement> Clone() const  = 0;	
	template<typename T>
    T To() const { return ::To<T>(Get()); }
private:
//	virtual constexpr bool operator==(const IElement& e) const = 0;
//	virtual constexpr std::strong_ordering operator<=>( const IElement& e) const noexcept = 0;
//	virtual constexpr const_iterator Begin() const = 0;
//	constexpr decltype(auto) End() { return data.begin() + size; }
//	constexpr decltype(auto) Size() { return size; }
//
//	constexpr bool operator==(const Element& e) const{ return Get() == e.Get(); };
//	constexpr std::strong_ordering operator<=>( const Element& e) const noexcept { return Get() <=> e.Get(); }
};
//--------------------------------Element------------------------------------------------

template<typename D>
class Element: public IElement
{
	using Derived = D;
public:
	inline static const std::string Identifier = "Element";
 	Element(const std::string& s): value{Derived::check(s)}, size{s.size()} { };
 	template<typename T>
	Element(T t): Element(std::to_string(t)) { };

	const std::string Get() const  {	return value; };	
	virtual std::unique_ptr<IElement> Clone() const  { return std::make_unique<Derived>(value); };	
	//const std::string () const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Size() { return size; }

	bool operator==(const Element& e) const{ return Get() == e.Get(); };
	std::strong_ordering operator<=>( const Element& e) const noexcept { return Get() <=> e.Get(); }
private:
	std::string value;
	std::size_t size;
};

std::ostream& operator<<(std::ostream& out, const IElement& e) {	return out<<e.Get();}

template <typename T>
concept ElementConcept = requires(T val) {	val.Get(); };
