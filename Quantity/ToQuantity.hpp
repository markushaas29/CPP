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
	using VT = typename T::ValueType;
	if(auto v = String_::TryTo<VT>(s))
		return T{(VT)v};
	return  T{1};
}
