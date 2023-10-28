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
	//virtual const std::string Get() const  = 0;	
	virtual constexpr char operator[](uint i) const = 0;
private:
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
template<typename Derived>
class Element: public IElement
{
public:
	using Type = Derived;
	inline static constexpr size_t N = 512;
	inline static const std::string Identifier = "Element";
	Element(): size{0}, data{std::array<char,N>{}} { };
 	Element(std::array<char,N> s): size{Len(s)}, data{s} { };
 	Element(const char* s): size{Len(Derived::check(s))}, data{Init<N>(Derived::check(s))} { };

	const std::string Value() const  {	return std::string(data.data()); };	
	//const std::string () const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Begin() { return data.begin(); }
	constexpr decltype(auto) End() { return data.begin() + size; }
	constexpr decltype(auto) Size() { return size; }

	constexpr char operator[](uint i) const { return data[i]; }
	constexpr bool operator==(const Element& e) const{ return Value() == e.Value(); };
	constexpr std::strong_ordering operator<=>( const Element& e) const noexcept { return Value() <=> e.Value(); }
private:
	std::size_t size;
	std::array<char,N> data;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Element<T>& e) {	return out<<e.Value();}

template <typename T>
concept ElementConcept = requires(T val) {	val.Value(); };
