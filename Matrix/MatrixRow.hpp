#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

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
	decltype(auto) operator*(const MatrixRow<T2>& r2)	{ return multiply(r2.tuple);  	}
	template<typename... Ts>
	decltype(auto) operator*(const std::tuple<Ts...>& t2)	{ return multiply(t2);  	}
	template<typename T2>
	decltype(auto) operator*(const T2& t2)	{ return multiply(tuple);  	}


	template<typename A>
	constexpr auto multiply(A arg) 
	{
		if constexpr ( IsTuple<A>)
		{
			int size = std::tuple_size_v<A>; 
			IsT<Throwing>(Format("Tuple size ",size, " is unequal ", Size))(size==Size);
			return calculate(arg);
		}
		else
			return calculate(arg);
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

	template<typename T2>
	auto calculate(const T2 t2) 	
	{  	
		if constexpr ( IsTuple<T2>)
			return calculateI<1>(t2, std::make_tuple(std::get<0>(tuple) * std::get<0>(t2) ));	
		else 
			return calculateI<1>(t2, std::make_tuple(std::get<0>(tuple) * t2 ));	
	}
	
	template<int N, typename T2>
	auto calculateI(const T2 t2, const auto r) 
	{
		if constexpr (N==Size)
        {
            return MatrixRow<decltype(r)>(r);
        }   
        else
        {
			if constexpr ( IsTuple<T2>)
			{
            	auto tN = std::tuple_cat(r,std::make_tuple(std::get<N>(tuple) * std::get<N>(t2) ));
            	return calculateI<N+1>(t2,tN);
			}
			else 
			{
            	auto tN = std::tuple_cat(r,std::make_tuple(std::get<N>(tuple) * t2 ));
            	return calculateI<N+1>(t2,tN);
			}
        }
	}
};
