#include <iostream>
#include "../../File/Repository.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Unit/Unit.hpp"
#include "../../Is/Is.hpp"
#include "../../String/Literal.hpp"
#include "../Reading.hpp"
#include "../Counter.hpp"
#include "../../Common/Analyzer.hpp"
#include "../../Components/IOComponents.hpp"
#include "../CounterConfiguration.hpp"
#include "../CounterContainer.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
	using RW = Reading<Work>;
	using W = RW::QuantityType;
	
	auto w = W{123};
	auto e = Q{2};
	auto dt = DT{1,2,2003};
	auto rw = RW{w,dt};
	
	auto w2 = W{124};
	auto dt2 = DT{1,2,2004};
	auto rw2 = RW{w2,dt2};
	
	auto w3 = W{148};

	std::unique_ptr<IIn> io = std::make_unique<InImpl<int>>();
	using T = InImpl<int>::Type;
	auto b = std::is_same<InImpl<int>,T>::value;
	auto b1 = std::is_same<int,T>::value;
	std::cout<<CG1::Identifier;
	auto lit = Literal{CG1::TypeIdentifier};
	using Rep = CSV::Repository<CSV::CounterRepositoryConfiguration>;
 	Rep::Instance();

	assert(b);
	assert(b1);
	CEHall::Display(std::cout);
	CEHall::Instance().Exec(std::cin, std::cout);

	using A = Analyzer<CG1,int>;
	A::Get();

    	return 0;
}
