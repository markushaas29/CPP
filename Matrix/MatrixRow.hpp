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

template<typename T>
class MatrixRow 
{
public:
	using Tuple = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixRow";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	static int constexpr Size = std::tuple_size_v<Tuple>;

	MatrixRow(Tuple t): matrix(t) {}
	template<int N>
	auto At() const { return std::get<N>(matrix); } 
private:
	Tuple matrix;
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

	friend std::ostream& operator<<(std::ostream& s, const MatrixRow& me) {	return s<<"{ " <<me.print(me.matrix,s)<<" }";	}
};
