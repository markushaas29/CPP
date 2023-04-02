#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
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
	constexpr Element(std::array<char,N> s): size{constLen(s)}, data{s} { };
	constexpr Element(const char* s): size{constLen(Derived::check(s))}, data{constInit(Derived::check(s))} { };

	const std::string Value() const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Begin() { return data.begin(); }
	constexpr decltype(auto) End() { return data.begin() + size; }
	constexpr decltype(auto) Size() { return size; }

	constexpr decltype(auto) operator[](uint i) { return data[i]; }
	constexpr bool operator==(const Element& e) const{ return Value() == e.Value(); };
	constexpr std::strong_ordering operator<=>( const Element& e) const noexcept { return Value() <=> e.Value(); }
private:
	std::size_t size;
	constexpr std::size_t constLen(const char * a)
	{
		const char* end = a;
		while(*end) ++end;
		return end - a;
	}
	
	constexpr std::size_t constLen(std::array<char,N> a)
	{
		auto end = a.cbegin();
		while(*end) ++end;
		return end - a.cbegin();
	}
	
	constexpr std::array<char,N> constInit(const char * a)
	{
		std::array<char,N> result = {};
		int i = 0;
		const char* end = a;
		while(*end) 
		{
			result[i++] = *end;
			++end;
		}
		return result;
	}
	std::array<char,N> data;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Element<T>& e) {	return out<<e.Value();}

#endif
