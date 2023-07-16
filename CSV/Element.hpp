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

#ifndef ELEMENT_H
#define ELEMENT_H

//--------------------------------Element------------------------------------------------
template<typename Derived>
class Element
{
public:
	using Type = Derived;
	inline static constexpr size_t N = 512;
	inline static const std::string Identifier = "Element";
	constexpr Element(): size{0}, data{std::array<char,N>{}} { };
	constexpr Element(std::array<char,N> s): size{Len(s)}, data{s} { };
	constexpr Element(const char* s): size{Len(Derived::check(s))}, data{Init<N>(Derived::check(s))} { };

	const std::string Value() const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Begin() { return data.begin(); }
	constexpr decltype(auto) End() { return data.begin() + size; }
	constexpr decltype(auto) Size() { return size; }

	constexpr decltype(auto) operator[](uint i) { return data[i]; }
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

#endif
