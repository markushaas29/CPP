#include "../String/To/To.hpp"
#pragma once

//template<typename T, typename U>
//decltype(auto) ToQuantity(std::istream& is, U u = U{})
template<typename T>
decltype(auto) ToQuantity(std::istream& is)
{
	std::string s;
	std::getline(is,s);
	if(!s.size())
		return T{0};
	if(auto v = String_::TryTo<typename T::ChronoValueType>(s))
		return T{(uint)v};
	return  T{1};
}
