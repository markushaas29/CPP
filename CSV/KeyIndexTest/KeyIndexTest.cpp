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
    std::vector<std::string> v123;
    v123.push_back("1");
    v123.push_back("2");
    v123.push_back("3");
    std::vector<std::string> v45;
    v123.push_back("4");
    v123.push_back("5");
    
    //~ ki12.SetKeyPatterns(v123.cbegin(), v123.cend());
    auto ki12 = KeyIndex(v123.cbegin(), v123.cend());
    bool result = ki12.Check(v45);
	assert(!result);
	assert(!ki12.Valid());
	assert(!(bool)ki12);
    return 0;
}
