#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixFilter.hpp"
#include "MatrixFilters.hpp"
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
	decltype(auto) operator()() const
	{
		auto mS22 = matrix.Slices(4,6,7,11);
        std::cout<<"MS 22"<<mS22<<std::endl;

        auto ms22F = MatrixFilter<decltype(mS22)>(mS22);
        UniqueElements<decltype(mS22)> ue;
        auto uni = ms22F(2,[&](const auto& s) { return ue(s);});
        std::cout<<"UNI: "<<ue<<std::endl;
	}
private:
	MatrixType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixAnalyzer& me) { return s;  }
};
