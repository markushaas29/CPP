#include "../To/To.hpp"
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

template<typename T, typename U = void>
decltype(auto) ToQuantity(std::istream& is, T t = T{})
{
	std::string s;
	std::getline(is,s);
	using VT = typename T::ValueType;
	if(!s.size())
		return t;
	if(auto v = TryTo<VT>(s))
		return T{(VT)v};
	return  t;
}

struct Input{};

template<typename T, Input>
T ToQuantity(std::istream& is, const T& t)
{
	std::cout<<t<<std::endl;
	return ToQuantity<T,void>(is,t);
}


