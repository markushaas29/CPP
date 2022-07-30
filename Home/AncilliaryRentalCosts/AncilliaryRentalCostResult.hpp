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
};


template<typename S,typename D, typename Q>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemResult<S,D,Q>& s){	return s.Display(out);	}

#endif
