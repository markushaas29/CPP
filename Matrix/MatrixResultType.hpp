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

template<typename,typename,bool,bool> class MatrixResultType;

template<typename LT, typename RT, bool LTTup, bool RTTup>
class MatrixResultTypeBase 
{
public:
	using Left = LT;
	using Right = RT;
	using Type = MatrixResultType<Left,Right,LTTup, RTTup>;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
protected:
	template<typename L, typename R>
	struct mul
	{
		using Type = decltype(std::declval<L>() * std::declval<R>());
	};
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixResultTypeBase& me) { return s;  }
};

template<typename LT, typename RT, bool LTup = IsTuple<LT>, bool RTup = IsTuple<RT>>
class MatrixResultType : MatrixResultTypeBase<LT,RT,LTup,RTup>
{
public:
	using Left = LT;
	using Right = RT;
	using Base = MatrixResultTypeBase<LT,RT,LTup,RTup>;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = LTup ? std::tuple_size_v<LT> : 1;
	
	static constexpr auto multiply() 
	{
		if constexpr ( IsTuple<Left>)
			return create<std::tuple_size_v<Left>>();
		else
		{
			using R = decltype(std::declval<Left>() * std::declval<Right>());
			return typename Base::mul<Left,Right>::Type{1};
		}
	} 	
private:
	template<int M>
	static auto create() 
	{
		using Type = std::tuple_element_t<0, Left>;
		return create<1,M>(std::make_tuple(typename Base::mul<Type,Right>::Type{1}));
	}

	template<int N, int M>
	static auto create(auto t) 
	{
		if constexpr (N==M)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Left>;
			return create<N+1,M>(std::tuple_cat(t,std::tuple<typename Base::mul<Type,Right>::Type>{1}));
		}
	} 
};
