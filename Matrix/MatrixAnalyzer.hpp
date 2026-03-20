#include <memory>
#include <tuple>
#include <vector>
#include <iomanip>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixFilter.hpp"
#include "MatrixFilters.hpp"
#include "MatrixInitializer.hpp"
#include "MatrixProjector.hpp"
#include "M3.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../CSV/Elements.hpp"

#pragma once

template<typename T>
class MatrixAnalyzer 
{
public:
	using MatrixType = T;
	using Type = T;
	using TupleType = std::tuple<IBAN,Name,Quantity<Sum>>;	
	inline static constexpr const char TypeIdentifier[] = "MatrixAnalyzer";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	MatrixAnalyzer(MatrixType m): filter(m) {}
	decltype(auto) operator()() const
	{
		auto mS22 = filter().Cols(4,6,7,11);
        auto ms22F = MatrixFilter<decltype(mS22)>(mS22);
        UniqueElements<decltype(mS22)> ue;
        auto uni = ms22F(2,[&](const auto& s) { return ue(s);});

		auto ibans = ue();

		std::vector<std::string> vec;

		std::vector<typename MatrixInitializer<2,std::string>::MatrixType> mx;

		for(auto i : ibans)
		{
			auto iban = IBAN(i);
			std::vector<std::string> v;
			if(iban.Valid())
			{
				v.push_back(i);
				vec.push_back(i);
			}
		}

		return vec;
	}
private:
	MatrixFilter<MatrixType> filter;

	decltype(auto) form(double number) const
	{
		std::ostringstream oss;
		oss << std::setprecision(2)<<std::fixed << number;
		return oss.str();
	}
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixAnalyzer& me) { return s;  }
};
