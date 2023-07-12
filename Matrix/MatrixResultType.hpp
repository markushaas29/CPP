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
	
	constexpr auto create() const
	{
		if constexpr ( IsTuple<Left> || IsTuple<Right>)
		{	
			std::cout<<"Tuple"<<std::endl;
			return 3;
		}
		else
		{
			using R = decltype(std::declval<Left>() + std::declval<Right>);
			return R{1};
		}
	} 	
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	//template<int N, typename Iterator>
//	auto parseIntern(auto t, Iterator begin, Iterator end) 
//	{
//		if constexpr (N==Size)
//			return t;
//		else
//		{
//			using Type = std::tuple_element_t<N, Tuple>;
//			auto s = begin + N;
//			if constexpr (N==0)
//			{
//				auto tN =  std::make_tuple(MatrixElement<Type>(*s));
//				return parseIntern<N+1>(tN,begin,end);
//			}
//			else
//			{
//				auto tN = std::tuple_cat(t,std::tuple<MatrixElement<Type>>{*s});
//				return parseIntern<N+1>(tN,begin,end);
//			}
//		}
//	} 
	friend std::ostream& operator<<(std::ostream& s, const MatrixResultType& me) { return s;  }
};
