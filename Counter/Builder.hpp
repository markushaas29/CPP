#include <map>
#include <chrono>
#include <memory>
#include <tuple>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename I, typename... T>
class Builder
{
	using Tup = std::tuple<T...>;
public:
	inline static constexpr const char TypeIdentifier[] = "Builder";
	inline static constexpr Literal TypeId{TypeIdentifier};

	Builder() = delete;
	decltype(auto) operator()() { return exec<0>(std::make_unique<std::vector<I>>()); }
private:
	I* interface;
	friend std::ostream& operator<<(std::ostream& s, const Builder& c){return s;}
	 
	template<size_t N>
	auto exec(auto&& res)
	{
		if constexpr (std::tuple_size<Tup>()==N)
			return res;
		else
		{
			using Type = std::tuple_element_t<N,Tup>;
			std::cout<<Type::Identifier<<std::endl;
			interface->Register(Type::Identifier,&Type::Make);
			exec<N+1>(res);
		}
	}
};
