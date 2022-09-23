#include <memory>
#include <vector>
#include "../../Calculator/CalculatorResult.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../Quantity/Quantity.h"

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename I,typename S, typename Q,typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using ItemType = I;
	using StageType = S;
	using SumType = Quantity<Sum>;
	using QuantityType = typename Q::TQuantity;
	using ResultType = decltype(QuantityRatio::Calculate(std::declval<QuantityType>(),std::declval<QuantityType>(),std::declval<SumType>()));
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t, const QuantityType& n, const QuantityType& d, const SumType& s, const DateTimes::Year y): transfers{std::move(t)}, result{QuantityRatio::Calculate(n,d,s)},year{y}, numerator{n},denominator{d},sum{s} {  };
	AncilliaryRentalCostItemResult():year{2000}, result{} {};
	std::ostream& Display(std::ostream& os) const
	{
		os<<StageType::StageName<<"\t"<<ItemType::Name<<"\t"<<this->year<<std::endl;
		os<<"Numerator: \t\t"<<numerator<<std::endl;
		os<<"Denominator: \t"<<denominator<<std::endl;
		os<<"Sum: \t"<<sum<<std::endl;
		os<<"Result: \t"<<result<<std::endl;
		os<<"\nTransfers: \t"<<std::endl;
		for(auto t : *transfers)
			os<<*t<<std::endl;
		return os;
	}
private:
	const DateTimes::Year year;
	const QuantityType numerator;
	const QuantityType denominator;
	const SumType sum;
	TransfersPtr transfers;
	ResultType result;
};


template<typename S,typename D, typename Q, typename A>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemResult<S,D,Q,A>& s){	return s.Display(out);	}

#endif
