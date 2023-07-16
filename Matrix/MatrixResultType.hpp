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
#include "../Calculator/Operations.hpp"
#pragma once


template<typename P> 
concept PointerConcept = requires(P p)
{
	*p;
	p.get(); 
	p.reset();
};

template<typename LT,typename RT,bool LTup = IsTuple<LT>, bool RTup = IsTuple<RT>> class MatrixResultType;

template<uint N, typename T>
decltype(auto) GetValue(T t, const auto& v)
{
	using type = std::remove_reference<decltype(t)>::type;
	if constexpr ( IsTuple<T>)
	{
		using TY=std::remove_reference<decltype(std::get<N>(t).Get())>::type;
		return typename TY::ValueType(v);
	}
	else
	{
		return typename T::ValueType(v);
	}
}

template<typename LT, typename RT, uint S, bool LTTup, bool RTTup>
class MatrixResultTypeBase 
{
public:
	using Left = LT;
	using Right = RT;
	using Type = MatrixResultType<Left,Right,LTTup, RTTup>;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = S;
protected:
	template<typename L, typename R>
	struct mul
	{
		using Type = decltype(Multiplication::Calculate(std::declval<L>(), std::declval<R>()));
	};
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixResultTypeBase& me) { return s;  }
};

template<typename LT, typename RT>
class MatrixResultType<LT,RT,false,false>: public MatrixResultTypeBase<LT,RT,1,false,false>
{
public:
	using Left = LT;
	using Right = RT;
	using Base = MatrixResultTypeBase<LT,RT,1,false,false>;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	
	static constexpr auto multiply() 
	{
		using R = decltype(Multiplication::Calculate(std::declval<Left>(), std::declval<Right>()));
		return typename Base::mul<Left,Right>::Type();
	} 	
};

template<typename LT, typename RT>
class MatrixResultType<LT,RT,true,false>: public MatrixResultTypeBase<LT,RT,std::tuple_size_v<LT>,true,false>
{
public:
	using Left = LT;
	using Right = RT;
	using Base = MatrixResultTypeBase<LT,RT,std::tuple_size_v<LT>,true,false>;
	inline static constexpr const char TypeIdentifier[] = "MatrixResultType";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	
	static constexpr auto multiply() 
	{
		if constexpr ( IsTuple<Left>)
			return create<std::tuple_size_v<Left>>();
		else
		{
			using R = decltype(Multiplication::Calculate(std::declval<Left>(), std::declval<Right>()));
			return typename Base::mul<Left,Right>::Type();
		}
	} 	
private:
	template<int M>
	static auto create() 
	{
		using Type = std::tuple_element_t<0, Left>;
		return create<1,M>(std::make_tuple(typename Base::mul<Type,Right>::Type()));
	}

	template<int N, int M>
	static auto create(auto t) 
	{
		if constexpr (N==M)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Left>;
			return create<N+1,M>(std::tuple_cat(t,std::tuple<typename Base::mul<Type,Right>::Type>()));
		}
	} 
};
