#include <sstream>
#include "../ParseResult.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Wrapper/Wrapper.hpp"

#pragma once
template<typename D, typename T>
struct MakeBase
{
	using Target = T;
	using Type = D;
	using ResultType = ParseResult<T>;

};

template<typename T=std::string>
class TryMake: public MakeBase<TryMake<T>,T>
{
	using Base = MakeBase<TryMake<T>,T>;
public:
	decltype(auto) operator()(std::istream& arg)
	{
		typename Base::Target result{};
		if(!(arg >> result) || !(arg >> std::ws).eof())
			return typename Base::ResultType();
		return typename Base::ResultType(result);
	}
};

template<typename T>
class Make: public MakeBase<Make<T>,T>
{
public:
	decltype(auto) operator()(std::istream& is)
	{
		auto m = TryMake<T>();
		auto result = m(is);
		if(!result.Valid)
			throw std::runtime_error("Make() failed");
		return result;
	}
};
