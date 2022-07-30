#include <memory>
#include <vector>

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename I,typename S, typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using ItemType = I;
	using StageType = S;
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t): transfers{std::move(t)}{  };
	AncilliaryRentalCostItemResult() = default;
	//~ AncilliaryRentalCostItemResult<S,T>& operator=(const AncilliaryRentalCostItemResult<S,T>& ) = default;
	std::ostream& Display(std::ostream& os) const
	{
		os<<StageType::Name<<"\t"<<ItemType::Name<<std::endl;
		for(auto t : *transfers)
			os<<*t<<std::endl;
		return os;
	}
private:
	TransfersPtr transfers;
};


template<typename S,typename D, typename Q>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemResult<S,D,Q>& s){	return s.Display(out);	}

#endif
