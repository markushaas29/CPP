#include <memory>
#include <tuple>
#include <vector>
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#pragma once

template<typename T>
class MatrixParser 
{
public:
	using Tuple = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixParser";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;
	
	template<typename Iterator>
	auto Parse(Iterator begin, Iterator end) 
	{
		if constexpr ( IsTuple<T>)
		{	
			IsT<Throwing>("Parse")(std::distance(begin,end)==Size);
			return parseIntern<0>(Tuple{},begin,end);
		}
		else
		{
			if constexpr (ElementConcept<T>) 
				return T{*begin};
			else
			{
				auto first = ParseElement(*begin);
				std::vector<decltype(first)> result = { first };
				std::for_each(begin+1,end,[&](auto e) { result.push_back(ParseElement(e)); } );
				return result;
			}
		}
	} 	
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<int N, typename Iterator>
	auto parseIntern(auto t, Iterator begin, Iterator end) 
	{
		if constexpr (N==Size)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Tuple>;
			auto s = begin + N;
			if constexpr (N==0)
			{
				auto tN =  std::make_tuple(MatrixElement<Type>(*s));
				return parseIntern<N+1>(tN,begin,end);
			}
			else
			{
				auto tN = std::tuple_cat(t,std::tuple<MatrixElement<Type>>{*s});
				return parseIntern<N+1>(tN,begin,end);
			}
		}
	} 
	friend std::ostream& operator<<(std::ostream& s, const MatrixParser& me) { return s;  }
};
