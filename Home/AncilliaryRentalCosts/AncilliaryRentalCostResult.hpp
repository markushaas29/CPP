#include <memory>
#include <vector>
#include "../../Calculator/CalculatorResult.hpp"

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename I,typename S, typename Q,typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using ItemType = I;
	using StageType = S;
	using QuantityType = Q;
	using ResultType = Result<QuantityRatio,QuantityType>;
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t, const DateTimes::Year y): transfers{std::move(t)}, year{y}{  };
	AncilliaryRentalCostItemResult():year{2000} {};
	std::ostream& Display(std::ostream& os) const
	{
		os<<StageType::Name<<"\t"<<ItemType::Name<<"\t"<<this->year<<std::endl;
		for(auto t : *transfers)
			os<<*t<<std::endl;
		return os;
	}
private:
	const DateTimes::Year year;
	TransfersPtr transfers;
	//~ ResultType result;
};


template<typename S,typename D, typename Q, typename A>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemResult<S,D,Q,A>& s){	return s.Display(out);	}

#endif
