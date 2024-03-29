#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixRow.hpp"
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
	using ProjectionType = P;
	inline static constexpr const char TypeIdentifier[] = "MatrixProjector";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Order = MatrixType::Order;

	MatrixProjector(MatrixType m): matrix(m) {}

	decltype(auto) operator[] (size_t i) const 
	{ 
		if constexpr ( IsTuple<ProjectionType> && MatrixType::Order == 2)
		{	
			IsT<Throwing>("Tuple Size unequal Cols")(matrix.Cols()==std::tuple_size_v<ProjectionType>);
			return MatrixRow<ProjectionType>(createTupleProjection<0>(i,std::make_tuple(0)));
		}
		else
		{
			if constexpr (MatrixConcept<MatrixType> && MatrixType::Order>1)
			{
				auto m = matrix[i];
				return MatrixProjector<decltype(m),ProjectionType>(m);
			}
			else
			{
				if constexpr ( !IsTuple<ProjectionType>)
					return ProjectionType(matrix[i]);
			}
		}
	}
	template<int N>
	decltype(auto)  Col() const
	{
		IsT<Throwing>(Format("Index ",N," exceeds extent ", matrix.Cols()))(matrix.Cols()>N);
		auto s = matrix.Col(N);
		if constexpr ( IsTuple<ProjectionType> && MatrixType::Order==2)
		{
			using Type = std::tuple_element_t<N, ProjectionType>;
			return MatrixProjector<decltype(s),Type>(s);
		}
		else
			return MatrixProjector<decltype(s),ProjectionType>(s);
	}
private:
	MatrixType matrix;
	template<typename> friend class MatrixRow;
	template<typename U> using IsT =  Is<U,LiteralType>;
	
	template<int N>
	auto createTupleProjection(size_t i, auto t) const
	{
		if constexpr (N==std::tuple_size_v<ProjectionType>)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, ProjectionType>;
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

	friend std::ostream& operator<<(std::ostream& s, const MatrixProjector& me) 
	{
		s<<"{ ";
		if constexpr (MatrixType::Order==2)
		{
			s<<"\n";
			for(auto i=0; i<me.matrix.Rows(); ++i)
				s<<me[i];
		}
		if constexpr (MatrixType::Order==1)
		{
			for(auto i=0; i<me.matrix.Rows(); ++i)
				s<<me[i]<<(i+1 == me.matrix.Rows()? "" : ", ");
		}
		s<<" }";
		return s;  
	}
};
