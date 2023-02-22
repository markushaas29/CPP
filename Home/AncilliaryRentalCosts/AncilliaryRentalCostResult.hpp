#include <memory>
#include <vector>
#include "../../Calculator/CalculatorResult.hpp"
#include "../../Calculator/Operations.hpp"
#include "../../Quantity/Quantity.hpp"

#ifndef ANCILLIARYRENTALCOSTITEM_HPP
#define ANCILLIARYRENTALCOSTITEM_HPP

template<typename Q>
class ItemFraction
{
public:
	using QuantityType = Q;
	using Type = ItemFraction<Q>;
	using ResultType = decltype(QuantityFraction::Calculate(std::declval<QuantityType>(),std::declval<QuantityType>(),std::declval<Quantity<Sum>>()));
	constexpr ItemFraction(QuantityType n = QuantityType{0}, QuantityType d = QuantityType{1}, Quantity<Sum> s = Quantity<Sum>{1}): num{n}, denom{d}, sumValue{s}, result{QuantityFraction::Calculate(n,d,s)}{};
	constexpr ItemFraction(ItemFraction&& i) : num{i.Num}, denom{i.Denom}, sumValue{i.SumValue}, result{i.result} {  i = QuantityFraction{}; };
	constexpr ItemFraction(const ItemFraction& i): num{i.num}, denom{i.denom}, sumValue{i.sumValue}, result{i.result}{ };
	constexpr ItemFraction& operator=(ItemFraction&& other) = default;
	
	std::ostream& Display(std::ostream& os) const
	{
		os<<"Numerator: \t\t"<<num<<"\n";
		os<<"Denominator: \t"<<denom<<"\n";
		os<<"Sum: \t"<<sumValue<<"\n";
		os<<"Result: \t"<<result<<"\n";
		
		return os;
	}
	
	constexpr bool operator==(const Type& i) const{ return (num == i.num) && (denom == i.denom) && (sumValue == i.sumValue); };
	constexpr decltype(auto) operator<=>( const Type& i) const noexcept { return sumValue <=> i.sumValue; }
private:	
	QuantityType num;
	QuantityType denom;
	Quantity<Sum> sumValue;
	ResultType result;
};

template<typename Q>
std::ostream& operator<<(std::ostream& out, const ItemFraction<Q>& s){	return s.Display(out);	}

template<typename I,typename F,typename S, typename Q,typename T>
class AncilliaryRentalCostItemResult
{
public:
	using Type = T;
	using ItemType = I;
	using StageType = S;
	using SumType = Quantity<Sum>;
	using QuantityType = typename Q::TQuantity;
	using FractionType = F;
	using ResultType = decltype(QuantityFraction::Calculate(std::declval<QuantityType>(),std::declval<QuantityType>(),std::declval<SumType>()));
	using Transfers = std::vector<std::shared_ptr<typename Type::TransferType>>;
	using TransfersPtr = std::unique_ptr<Transfers>;
	AncilliaryRentalCostItemResult(TransfersPtr&& t, FractionType&& f,const QuantityType& n, const QuantityType& d, const SumType& s, const DateTimes::Year y): transfers{std::move(t)}, fraction{f},result{QuantityFraction::Calculate(n,d,s)},year{y}, numerator{n},denominator{d},sum{s} {  };
	AncilliaryRentalCostItemResult():year{2000}, result{} {};
	std::ostream& Display(std::ostream& os) const
	{
		os<<StageType::StageName<<"\t"<<ItemType::TypeIdentifier<<"\t"<<year<<"\n";
		os<<"Fraction: \t\t"<<fraction<<"\n";
		os<<"Numerator: \t\t"<<numerator<<"\n";
		os<<"Denominator: \t"<<denominator<<"\n";
		os<<"Sum: \t"<<sum<<"\n";
		os<<"Result: \t"<<result<<"\n";
		os<<"Result sum: \t"<<result.Get()<<"\n";
		os<<"\nTransfers: \t"<<"\n";
		for(auto t : *transfers)
			os<<*t<<"\n";
		return os;
	}
	
	decltype(auto) Get() { return result.Get(); }
private:
	const FractionType fraction;
	const DateTimes::Year year;
	const QuantityType numerator;
	const QuantityType denominator;
	const SumType sum;
	TransfersPtr transfers;
	ResultType result;
};

template<typename S,typename F,typename D, typename Q, typename A>
std::ostream& operator<<(std::ostream& out, const AncilliaryRentalCostItemResult<S,F,D,Q,A>& s){	return s.Display(out);	}

#endif
