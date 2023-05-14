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
	using ResultType = ParseResult<Target>;

};

template<typename T=std::string>
class TryMake
{
public:
	decltype(auto) operator()(std::istream& arg)
	{
		T result;
		if(!(arg >> result) || !(arg >> std::ws).eof())
			return ParseResult<T>();
		return ParseResult<T>(result);
	}
};

template<typename T>
class Make
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
