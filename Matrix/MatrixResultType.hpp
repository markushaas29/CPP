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

template<typename LT, typename RT>
class MatrixResultType 
{
public:
	using Left = LT;
	using Right = RT;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = 1; //std::tuple_size_v<Tuple>;
	
	static constexpr auto multiply() 
	{
		if constexpr ( IsTuple<Left>)
		{	
			return create<std::tuple_size_v<Left>>();
		}
		else
		{
			using R = decltype(std::declval<Left>() * std::declval<Right>());
			return typename mul<Left,Right>::Type{1};
		}
	} 	
private:
	template<typename L, typename R>
	struct mul
	{
		using Type = decltype(std::declval<L>() * std::declval<R>());
	};

	template<typename U> using IsT =  Is<U,LiteralType>;
	
	template<int M>
	static auto create() 
	{
		using Type = std::tuple_element_t<0, Left>;
		return create<1,M>(std::make_tuple(typename mul<Type,Right>::Type{1}));
	} 
	template<int N, int M>
	static auto create(auto t) 
	{
		if constexpr (N==M)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Left>;
			return create<N+1,M>(std::tuple_cat(t,std::tuple<typename mul<Type,Right>::Type>{1}));
		}
	} 
	friend std::ostream& operator<<(std::ostream& s, const MatrixResultType& me) { return s;  }
};
