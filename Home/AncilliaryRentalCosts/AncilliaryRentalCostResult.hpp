#include <memory>
#include <vector>

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t): transfers{std::move(t)}{};
private:
	TransfersPtr transfers;
};

#endif
