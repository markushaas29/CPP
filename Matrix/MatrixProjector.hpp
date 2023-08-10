#include <memory>
#include <tuple>
#include <vector>
#include "MatrixElement.hpp"
#include "Matrix.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

template<typename T, typename P>
class MatrixProjector 
{
public:
	using MatrixType = T;
	using Tuple = T;
	using Type = T;
	using ProjectionType = P;
	inline static constexpr const char TypeIdentifier[] = "MatrixProjector";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;

	MatrixProjector(MatrixType m): matrix(m) {}
	
	decltype(auto) operator[] (size_t i) const 
	{ 
		if constexpr (MatrixConcept<MatrixType> && MatrixType::Order>1)
		{
			auto m = matrix[i];
			return MatrixProjector<decltype(m),ProjectionType>(m);
		}
		else
			return ProjectionType(matrix[i]);
	}

	template<typename I>
	auto Parse(std::vector<I> v) const
	{
		if constexpr ( IsTuple<T>)
		{	
			IsT<Throwing>("Parse")(std::distance(v.cbegin(),v.cend())==Size);
			return parseIntern<0>(Tuple{},v.cbegin(),v.cend());
		}
		else
		{
			if constexpr (ElementConcept<T>) 
			{
				std::vector<T> result;
				if constexpr (PointerConcept<decltype(v[0])>)
				{
					std::for_each(v.cbegin(),v.cend(),[&](auto e) { result.push_back(T(*e)); } );
				}
				else
				{
					std::for_each(v.cbegin(),v.cend(),[&](auto e) { result.push_back(T(e)); } );
				}
				return result;
			}
			else
			{
				auto first = ParseElement(v[0]);
				std::vector<decltype(first)> result = { first };
				std::for_each(v.cbegin()+1,v.cend(),[&](auto e) { result.push_back(ParseElement(e)); } );
				return result;
			}
		}
	} 	
private:
	MatrixType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<int N, typename Iterator>
	auto parseIntern(auto t, Iterator begin, Iterator end) const
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
	friend std::ostream& operator<<(std::ostream& s, const MatrixProjector& me) { return s;  }
};
