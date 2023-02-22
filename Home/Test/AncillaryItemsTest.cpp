#include <iostream>
#include "../../Common/ShortNames.hpp"
#include "../../Unit/Unit.hpp"
#include "../Reading.hpp"
#include "../StageContainer.hpp"
#include "../AncilliaryRentalCosts/AncilliaryRentalCostItems.hpp"
#include "../AncilliaryRentalCosts/AncilliaryRentalCostResult.hpp"
#include "../AncilliaryRentalCosts/AncilliaryRentalCostsContainer.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	using RW = Reading<Work>;
	using W = RW::QuantityType;
	
	auto w123 = W{123};
	constexpr auto w1 = W{1};
	constexpr auto w2 = W{2};
	constexpr  auto e4 = Q{4};
	auto e2 = Q{2};
	auto dt = DT{1,2,2003};
	auto y2022 = Y{2022};
	auto rw = RW{w123,dt};
	
	auto w124 = W{124};
	auto dt2 = DT{1,2,2004};
	auto rw2 = RW{w124,dt2};
	
	std::cout<<PT<Top>::Cause<<"\n";
	
	constexpr ItemFraction<W> ifD{};
	constexpr ItemFraction<W> ifD2{};
	constexpr ItemFraction<W> if124{w1,w2,e4};
	assert(ifD==ifD2);
	assert(ifD!=if124);
	assert(ifD<if124);
	assert(if124>ifD);
	assert(ifD<=ifD2);
	assert(ifD>=ifD2);


	//~ PT<Top>::Calculate(y2022);	
	//~ BI<Top>::Calculate(y2022);	
	WF<Top>::Calculate(y2022);	
	//~ CS<Top>::Calculate(y2022);	
	//~ SW<Top>::Calculate(y2022);	
	//~ PT<Top>::Calculate(y2022);	
	//~ BC<Top>::Calculate(y2022);	
	//~ HT<Top>::Calculate(y2022);	
	
    return 0;
}
