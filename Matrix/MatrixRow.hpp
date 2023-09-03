#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "MatrixRowConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Functional/Functional.hpp"

#pragma once

template<typename T, typename P> class MatrixProjector;

template<typename T>
class MatrixRow 
{
public:
	using Tuple = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixRow";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;

	MatrixRow(Tuple t): tuple(t) {}
	template<int N>
	auto At() const {	return std::get<N>(tuple); 	} 

	template<typename T2>
	decltype(auto) operator+(const MatrixRow<T2>& r2)	{ return Apply<Add>(r2.tuple);  	}
	template<typename... Ts>
	decltype(auto) operator+(const std::tuple<Ts...>& t2)	{ return Apply<Add>(t2);  	}
	template<typename T2>
	decltype(auto) operator+(const T2& t2)	{ return Apply<Add>(t2);  	}
	
	template<typename T2>
	decltype(auto) operator*(const MatrixRow<T2>& r2)	{ return Apply<Mul>(r2.tuple);  	}
	template<typename... Ts>
	decltype(auto) operator*(const std::tuple<Ts...>& t2)	{ return Apply<Mul>(t2);  	}
	template<typename T2>
	decltype(auto) operator*(const T2& t2)	{ return Apply<Mul>(t2);  	}
	template<typename T2, typename P>
	decltype(auto) operator*(const MatrixProjector<T2,P>& mp)	
	{ 
		if constexpr ( MatrixProjector<T2,P>::Order != 1)
			IsT<Throwing>(Format("Order ",MatrixProjector<T2,P>::Order, " is not 1"))(false);
		else
		{
			IsT<Throwing>(Format("Projector rows ",mp.matrix.Rows(), " is unequal ", Size))(Size==mp.matrix.Rows());
			return Apply<Mul>(mp[0]);
		}
	}
	
	template<template<typename,typename> class F, MatrixRowConcept A>
	constexpr auto Apply(A arg) 
	{
		int size = std::tuple_size_v<typename A::Tuple>; 
		IsT<Throwing>(Format("Tuple size ",A::Size, " is unequal ", Size))(A::Size==Size);
		return calculate<F>(arg);
	}

	template<template<typename,typename> class F, typename A>
	constexpr auto Apply(A arg) 
	{
		if constexpr ( IsTuple<A>)
		{
			int size = std::tuple_size_v<A>; 
			IsT<Throwing>(Format("Tuple size ",size, " is unequal ", Size))(size==Size);
			return calculate<F>(arg);
		}
		else
			return calculate<F>(arg);
	}
private:
	Tuple tuple;
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<class TupType, size_t... I>
	void print(const TupType& _tup, std::index_sequence<I...>, std::ostream& os) const
	{
	    os << "{";
	    (..., (os << (I == 0? "" : ", ") << std::get<I>(_tup)));
	    os << "}\n";
	}
	
	template<class... R>
	decltype(auto) print (const std::tuple<R...>& _tup, std::ostream& os) const	
	{   
		print(_tup, std::make_index_sequence<sizeof...(R)>(), os);	
		return os;
	}

	friend std::ostream& operator<<(std::ostream& s, const MatrixRow& me) 	{	return	me.print(me.tuple,s);	}


	template<template<typename,typename> class F, MatrixRowConcept A>
	auto calculate(const A arg) 
	{  	
		auto f = Func<F>(Func<Constant>(std::get<0>(tuple)), Func<Constant>(arg.template At<0>()));
		return calculateI<1,Mul>(arg, std::make_tuple(f() ));	
	}

	template<template<typename,typename> class F, typename T2>
	auto calculate(const T2 t2) 	
	{  	
		if constexpr ( IsTuple<T2>)
		{
			auto f = Func<F>(Func<Constant>(std::get<0>(tuple)), Func<Constant>(std::get<0>(t2)));
			return calculateI<1,Mul>(t2, std::make_tuple(f()));	
		}
		else
		{
			auto f = Func<F>(Func<Constant>(std::get<0>(tuple)), Func<Constant>(t2));
			return calculateI<1,Mul>(t2, std::make_tuple(f() ));	
		}
	}

	template<int N, template<typename,typename> class F, MatrixRowConcept R>
	auto calculateI(const R r, const auto t) 
	{
		if constexpr (N==Size)
            return MatrixRow<decltype(t)>(t);
        else
        {
			auto f = Func<F>(Func<Constant>(std::get<N>(tuple)), Func<Constant>(r.template At<N>()));
           	auto tN = std::tuple_cat(t,std::make_tuple(f()));
           	return calculateI<N+1, Mul>(r,tN);
		}
	}

	template<int N, template<typename,typename> class F,typename T2>
	auto calculateI(const T2 t2, const auto r) 
	{
		if constexpr (N==Size)
            return MatrixRow<decltype(r)>(r);
        else
        {
			if constexpr ( IsTuple<T2>)
			{
				auto f = Func<F>(Func<Constant>(std::get<N>(tuple)), Func<Constant>(std::get<N>(t2)));
            	auto tN = std::tuple_cat(r,std::make_tuple(f()));
            	return calculateI<N+1,F>(t2,tN);
			}
			else 
			{
				auto f = Func<F>(Func<Constant>(std::get<N>(tuple)), Func<Constant>(t2));
            	auto tN = std::tuple_cat(r,std::make_tuple(f()));
            	return calculateI<N+1,F>(t2,tN);
			}
        }
	}
};
