#include <memory>
#include <vector>

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using Transfers = std::vector<typename Type::DataType>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t): transfers{std::move(t)}{std::cout<<"RESULT"<<*(*transfers)[0]<<std::endl;};
private:
	TransfersPtr transfers;
};

#endif
