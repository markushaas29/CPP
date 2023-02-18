#include <iostream>
#include <limits>
#include <vector>
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
    auto k3 = Key("3");
    auto k12345 = Key("12345");
    std::cout<<k12345<<std::endl;
    assert(k1=="1");
    assert(k1==k_1);
    assert(k1!=k12345);
    assert(k1<k12345);

	constexpr uint i = 12;
	constexpr auto i12c = Index{i,"12"};
	auto i12 = Index{i};
	auto i2 = Index{(uint)2};
	auto i_12 = Index{(uint)12};
    assert(i12==i_12);
    assert(i12!=i2);
    
    assert(i12.Valid());
    assert((bool)i12);
    std::cout<<i12.Value()<<std::endl;
    
    auto iMax = Index{};
    assert(!iMax.Valid());
    assert(!(bool)iMax);
    assert(iMax.Get()==std::numeric_limits<unsigned int>::max());
    std::cout<<iMax.Value()<<std::endl;
    
    assert(i12==i_12);
    std::vector<std::string> v123 {"1", "2", "3"};
    std::vector<std::string> v45{"4","5"};
    std::vector<std::string> v7839{"7","8","3","9"};
    
    auto ki123 = KeyIndex(v123.cbegin(), v123.cend());
    std::cout<<ki123<<std::endl;

    bool result = ki123.Check(v45);
    std::cout<<ki123.GetKey()<<" / "<<k1.Value()<<std::endl;
	assert(k1==ki123);
	assert(ki123.KeysSize()==3);
	assert(ki123.StringId()=="KeyIndex");
	assert(!result);
	assert(!ki123.Valid());
	assert(!(bool)ki123);
	
    result = ki123.Check(v7839);
	assert(ki123.KeysSize()==3);
	assert(k3==ki123);
	assert(result);
	assert(ki123.Valid());
	assert((bool)ki123);
	assert((int)ki123.GetIndex()==2);
	
	auto ki_123 = KeyIndex(ki123);
    std::cout<<ki_123<<std::endl;
	assert(ki123.GetKey()==ki_123);
	assert(ki_123.KeysSize()==3);
	assert(ki_123.Identifier=="KeyIndex");
	assert(ki_123.StringId()=="KeyIndex");
	
	KeyIndex ki1{k1};
    std::cout<<ki1<<"TEST "<<k1.Value()<<std::endl;
	assert(ki1.GetKey()==k1);
	assert(ki1.KeysSize()==1);
	assert((int)ki1.GetIndex()==0);
	assert(ki1.StringId()==k1.Value());
	
    return 0;
}
