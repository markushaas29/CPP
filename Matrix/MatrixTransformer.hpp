#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "MatrixElement.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#pragma once

template<typename... T>
class MatrixTransformer 
{
public:
	using Tuple = std::tuple<T...>;
	inline static constexpr const char TypeIdentifier[] = "MatrixTransformer";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;
	auto Create(InputIterator begin, InputIterator end) 
	{
		IsT<Throwing>("Create")(std::distance(begin,end)==Size);
		return createIntern<0>(Tuple{},begin,end);
	} 	
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<int N>
	auto createIntern(auto t, InputIterator begin, InputIterator end) 
	{
		if constexpr (N==Size)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Tuple>;
			auto s = begin + N;
			if constexpr (N==0)
			{
				auto tN =  std::make_tuple(Type{*s});
				return createIntern<N+1>(tN,begin,end);
			}
			else
			{
				auto tN = std::tuple_cat(t,std::tuple<Type>{*s});
				return createIntern<N+1>(tN,begin,end);
			}
		}
	} 	
	friend std::ostream& operator<<(std::ostream& s, const MatrixTransformer& me) { return s;  }
};
