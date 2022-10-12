#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.h"
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


template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>   to_array_impl(T (&a)[N], std::index_sequence<I...>){  return { {a[I]...} };}
 
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]){  return to_array_impl(a, std::make_index_sequence<N>{}); }

namespace detail
{
    template<unsigned... digits>
    struct to_chars { static const char value[]; };

    template<unsigned... digits>
    constexpr char to_chars<digits...>::value[] = {('0' + digits)..., 0};

    template<unsigned rem, unsigned... digits>
    struct explode : explode<rem / 10, rem % 10, digits...> {};

    template<unsigned... digits>
    struct explode<0, digits...> : to_chars<digits...> {};
}

template<unsigned num>
struct num_to_string : detail::explode<num> {};

template<uint S>
constexpr decltype(auto) convertIntegerToChar(uint N)
{
    uint m = N;

    uint digit = 0;
    while (m) {
        digit++;
        m /= 10;
    }

    std::array<char,S> arr = {};
	std::array<char,S> arr1 = {};
 
    int index = 0;
    while (N) {
        arr1[++index] = N % 10 + '0';
        N /= 10;
    }
    
    
    std::cout<<index<<std::endl;

    for (uint i= 0; i < S; i++) {
        arr[i] = arr1[S - i -1];
		if(!(int)arr[i])
			arr[S-1] = '0';
    }
 
    return arr;
}

constexpr char NumToChar(uint i){ return (char)(i + '0'); }

template<uint N>
constexpr decltype(auto) IntToChars(uint num)
{
    std::array<char,N+1> result = {};
    uint i;
    uint E = (N-1) * 10;
    while(num >= E)
	{
		num = num - 10;
		++i;
	}
	
	result[N-2] = NumToChar(i);
	result[N-1] = NumToChar(num);
	result[N] = '\0';
	
	return result;
}
//--------------------------------Element------------------------------------------------
class Element
{
public:
	inline static constexpr size_t N = 512;
	inline static const std::string Identifier = "Element";
	constexpr Element(std::array<char,N> s): size{constLen(s)}, data{s} { };
	constexpr Element(const char* s): size{constLen(s)}, data{constInit(s)} { };

	const std::string Value() const  {	return std::string(data.data()); };	
	constexpr decltype(auto) Begin() { return data.begin(); }
	constexpr decltype(auto) End() { return data.begin() + size; }
	constexpr decltype(auto) Size() { return size; }
	constexpr decltype(auto) operator[](uint i) { return data[i]; }
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

std::ostream& operator<<(std::ostream& out, const Element& e) {	return out<<e.Value();}

//--------------------------------CreateElementNewPolicy------------------------------------------------

template<class ConcreteProduct, typename T = std::string>
class CreateElementNewPolicy
{
public:
	static Element* DoCreate(T param){	return new ConcreteProduct(param);	}
};

#endif
