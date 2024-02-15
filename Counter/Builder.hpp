#include <map>
#include <chrono>
#include <memory>
#include <tuple>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename I, template<typename> class T, typename... DS>
class Builder
{
	using Tup = std::tuple<DS...>;
	using Args = std::string;
public:
	inline static constexpr const char TypeIdentifier[] = "Builder";
	inline static constexpr Literal TypeId{TypeIdentifier};

	Builder() = delete;
	Builder(const std::string& a, std::shared_ptr<Factory<IToken>> f): args{a}, factory{f} {};
	decltype(auto) operator()() { return exec<0>(std::make_unique<std::vector<std::unique_ptr<I>>>()); }
private:
	const Args args;
	std::shared_ptr<Factory<IToken>> factory;
	friend std::ostream& operator<<(std::ostream& s, const Builder& c){return s;}
	 
	template<size_t N>
	auto exec(auto&& res)
	{
		if constexpr (std::tuple_size<Tup>()==N)
			return std::move(res);
		else
		{
			using Type = std::tuple_element_t<N,Tup>;
			res->push_back(std::make_unique<T<Type>>(args,factory));
			std::cout<<Type::Identifier<<std::endl;
			return exec<N+1>(res);
		}
	}
};
