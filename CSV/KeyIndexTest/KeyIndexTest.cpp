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
    auto ke = Key();
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
    
    std::cout<<"DEfault Index"<<std::endl;
    auto iMax = Index{};
    assert(!iMax.Valid());
    assert(!(bool)iMax);
    assert(iMax.Get()==std::numeric_limits<unsigned int>::max());
    std::cout<<iMax.Get()<<std::endl;
    std::cout<<iMax<<std::endl;
    
    assert(i12==i_12);
    std::vector<std::string> v123 {"1", "2", "3"};
    std::vector<std::string> v45{"4","5"};
    std::vector<std::string> v7839{"7","8","3","9"};
    
    auto kiIBAN = KeyIndex("IBAN");
    auto ki123 = KeyIndex(v123.cbegin(), v123.cend());
    auto ki45 = KeyIndex(v45.cbegin(), v45.cend());
    auto ki7839 = KeyIndex(v7839.cbegin(), v7839.cend());
    std::cout<<ki123<<std::endl;

	assert(kiIBAN.StringId()=="IBAN");

    std::cout<<"HasKey string"<<std::endl;
    bool result = ki123.HasKey("1");
	assert(result);
	result = ki123.HasKey("4");
	assert(!result);
    
    std::cout<<"HasKey vec"<<std::endl;
    result = ki123.HasKey(v123.cbegin(),v123.cend());
	assert(result);
	result = ki123.HasKey(v45.cbegin(),v45.cend());
	assert(!result);
	
    ki123.Reset();
    std::cout<<"Ki Reset: "<<ki123<<std::endl;
    assert(!ki123.Valid());
    assert(ki123.GetIndex()==iMax);
    assert(ki123.GetKey()==ke);
  
    std::cout<<"Ki Check 123: "<<ki123<<std::endl;
    result = ki123.Check(v123);
    std::cout<<ki123.GetKey()<<" / "<<k1.Value()<<std::endl;
	assert(k1==ki123);
	assert(ki123.KeysSize()==3);
	assert((int)ki123.GetIndex()==0);
	
	auto i0 = Index{0};
	assert(ki123.GetIndex()==i0);
	assert(ki123.StringId()=="KeyIndex");
	assert(result);
	assert(ki123.Valid());
	assert((bool)ki123);
  
    std::cout<<"Ki Check 45: "<<ki123<<std::endl;
    result = ki123.Check(v45);
    std::cout<<ki123.GetKey()<<" / "<<k1.Value()<<std::endl;
	assert(k1==ki123);
	assert(ki123.KeysSize()==3);
	assert((int)ki123.GetIndex()==0);
	
	assert(ki123.GetIndex()==i0);
	assert(!result);
	assert(ki123.Valid());
	assert((bool)ki123);
	
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
	assert(ki1.GetKey()==k1);
	assert(ki1.KeysSize()==1);
	assert((int)ki1.GetIndex()==0);
	//~ assert(ki1.StringId()==k1.Value());
	
	std::vector<decltype(ki45)> kiv {ki123,ki45,ki7839};
	auto kip = std::make_unique<std::vector<decltype(ki45)>>(std::move(kiv));
	
	KIC kic {std::move(kip)};
	std::vector<std::string> v149 {"1","4","9"};
	kic.Reset();
    std::cout<<"KIC check 1 4 9"<<std::endl;
    std::cout<<kic<<std::endl;
    assert(!kic.Valid());
	result = kic.Check(v149);
    assert(kic.Valid());
	assert(result);
    std::cout<<"KIC "<<kic<<std::endl;
	
    std::cout<<"KIC check 1 6 9"<<std::endl;
	std::vector<std::string> v169 {"1","6","9"};
	kic.Reset();
    assert(!kic.Valid());
	result = kic.Check(v169);
	assert(!result);
    assert(!kic.Valid());
    std::cout<<"KIC "<<kic<<std::endl;
	
	std::vector<std::string> v257 {"2","5","7"};
    std::cout<<"KIC check 2 5 7"<<std::endl;
	kic.Reset();
    assert(!kic.Valid());
	result = kic.Check(v257);
	assert(result);
    assert(kic.Valid());
    std::cout<<"KIC "<<kic<<std::endl;
	
    return 0;
}
