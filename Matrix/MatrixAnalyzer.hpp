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
class MatrixAnalyzer 
{
public:
	using MatrixType = T;
	using Type = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixAnalyzer";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	MatrixAnalyzer(MatrixType m): matrix(m) {}
private:
	MatrixType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixAnalyzer& me) { return s;  }
};
