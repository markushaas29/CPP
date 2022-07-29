#include <memory>
#include <vector>

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename I,typename S, typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using Item = I;
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t): transfers{std::move(t)}{ std::cout<<"Result"<<Item::Name<<"\t"<<S::Name<<std::endl; };
	AncilliaryRentalCostItemResult() = default;
	//~ AncilliaryRentalCostItemResult<S,T>& operator=(const AncilliaryRentalCostItemResult<S,T>& ) = default;
private:
	TransfersPtr transfers;
};

#endif
