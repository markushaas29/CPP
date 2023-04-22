#include "../String/To/To.hpp"
#pragma once

template<typename T>
concept ValueTypeConcept = requires(T)
{
	T::ValueType;
};

template<typename T>
T ToQuantity(std::istream& is, std::ostream& os)
{
	return T::Create(is,os);
}

template<typename T>
T ToQuantity(std::istream& is, const T& t)
{
	std::cout<<t<<std::endl;
	std::string s;
	std::getline(is,s);
	if(!s.size())
		return t;
	using VT = typename T::ValueType;
	if(auto v = String_::TryTo<VT>(s))
		return T{(VT)v};
	return  T{1};
}

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
