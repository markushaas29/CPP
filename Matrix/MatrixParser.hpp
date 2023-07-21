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


template<typename P> 
concept PointerConcept = requires(P p)
{
	*p;
	p.get(); 
	p.reset();
};

template<typename T, typename D>
class MatrixParser 
{
public:
	using Tuple = T;
	using Descriptor = D;
	inline static constexpr const char TypeIdentifier[] = "MatrixParser";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;
	
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
				auto tN =  std::make_tuple(MatrixElement<Type,Descriptor>(*s));
				return parseIntern<N+1>(tN,begin,end);
			}
			else
			{
				auto tN = std::tuple_cat(t,std::tuple<MatrixElement<Type,Descriptor>>{*s});
				return parseIntern<N+1>(tN,begin,end);
			}
		}
	} 
	friend std::ostream& operator<<(std::ostream& s, const MatrixParser& me) { return s;  }
};
