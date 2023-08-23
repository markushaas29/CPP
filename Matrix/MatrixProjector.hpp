#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
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
	using Tuple = P;
	using Type = T;
	using ProjectionType = P;
	inline static constexpr const char TypeIdentifier[] = "MatrixProjector";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;

	MatrixProjector(MatrixType m): matrix(m) {}
	
	decltype(auto) operator[] (size_t i) const 
	{ 
		if constexpr ( IsTuple<ProjectionType> && MatrixType::Order == 2)
		{	
			IsT<Throwing>("Tuple Size unequal Cols")(matrix.Cols()==Size);
			return createTupleProjection<0>(i,std::make_tuple(0));
		}
		else
		{
			if constexpr (MatrixConcept<MatrixType> && MatrixType::Order>1)
			{
				auto m = matrix[i];
				return MatrixProjector<decltype(m),ProjectionType>(m);
			}
			else
				return ProjectionType(matrix[i]);
		}
	}
private:
	MatrixType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<int N>
	auto createTupleProjection(size_t i, auto t) const
	{
		if constexpr (N==Size)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Tuple>;
			if constexpr (N==0)
			{
				auto tN =  std::make_tuple(Type(matrix[i][N]));
				return createTupleProjection<N+1>(i,tN);
			}
			else
			{
				auto tN = std::tuple_cat(t,std::tuple<Type>{matrix[i][N]});
				return createTupleProjection<N+1>(i,tN);
			}
		}
	} 
	template<class TupType, size_t... I>
	void print(const TupType& _tup, std::index_sequence<I...>, std::ostream& os) const
	{
	    os << "{";
	    (..., (os << (I == 0? "" : ", ") << std::get<I>(_tup)));
	    os << "}\n";
	}
	template<class... R>
	void print (const std::tuple<R...>& _tup, std::ostream& os) const	{   print(_tup, std::make_index_sequence<sizeof...(R)>(), os);	}
	friend std::ostream& operator<<(std::ostream& s, const MatrixProjector& me) 
	{
		for(auto i=0; i<me.matrix.Rows(); ++i)
		{
			me.print(me[i],s);
		}
		return s;  
	}
};
