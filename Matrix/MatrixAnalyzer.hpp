#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixFilter.hpp"
#include "MatrixFilters.hpp"
#include "MatrixInitializer.hpp"
#include "MatrixProjector.hpp"
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
		auto mS22 = filter().Slices(4,6,7,11);
        std::cout<<"MS 22"<<mS22<<std::endl;

        auto ms22F = MatrixFilter<decltype(mS22)>(mS22);
        UniqueElements<decltype(mS22)> ue;
        auto uni = ms22F(2,[&](const auto& s) { return ue(s);});

		auto ibans = ue();

        std::cout<<"UNI: "<<ue<<std::endl;

		std::vector<std::vector<std::string>> vec;

		for(auto i : ibans)
		{
			auto iban = IBAN(i);
			std::vector<std::string> v;
			if(iban.Valid())
			{
				v.push_back(i);
				auto r = filter(7,[&](const auto& s) { return s == iban.Value();});
				std::cout<<"\nIBAN: "<<iban<<std::endl;
				auto sl = r.Slices(4,6,11,10);
				v.push_back(sl[0][1]);
				v.push_back(std::to_string(r.ColSum(11)));
				std::cout<<"\nSum:\t"<<Quantity<Sum>(r.ColSum(11))<<std::endl;
				vec.push_back(v);
			}
		}

		
		auto r2 = filter(4,[&](const auto& s) { return !DateTimes::Date(s).Valid();});
		std::cout<<"Dates: \n"<<r2<<std::endl;
		auto rd = filter(4,[&](const auto& s) { return DateTimes::Get<DateTimes::Month>(DateTimes::Date(s)) == 1;});
		std::cout<<"Dates: \n"<<rd<<std::endl;
		std::cout<<"Date: \n"<<filter()[0]<<std::endl;
		std::cout<<"Date: \n"<<filter()[0][1]<<std::endl;

		auto mi = Init(vec);
		auto mp = MatrixProjector<decltype(mi()),TupleType>(mi());
		std::cout<<"Umsatz: \n"<<mp<<std::endl;
	}
private:
	MatrixFilter<MatrixType> filter;

	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixAnalyzer& me) { return s;  }
};
