#include <iostream>
#include "../../Common/ShortNames.hpp"
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../KeyIndex.hpp"
using namespace ShortNames;
using namespace CSV;

int main()
{       
    std::cout<<"START"<<std::endl;
    
    auto k1 = Key("1");
    auto k_1 = Key("1");
    auto k12 = Key("12");
    std::cout<<k1<<std::endl;
    assert(k1=="1");
    assert(k1==k_1);
    assert(k1!=k12);
    assert(k1<k12);

	constexpr uint i = 12;
	auto i12 = Index{i};
	auto i2 = Index{(uint)2};
	auto i_12 = Index{(uint)12};
    assert(i12==i_12);
    assert(i12!=i2);
    assert(i12>i2);
    std::cout<<i12.Value()<<std::endl;
    
    assert(i12==i_12);
		
    return 0;
}
