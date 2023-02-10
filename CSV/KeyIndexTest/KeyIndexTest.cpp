#include <iostream>
#include "../../Common/ShortNames.hpp"
#include "../Elements.hpp"
#include "../KeyIndex.hpp"
using namespace ShortNames;

int main()
{       
    std::cout<<"START"<<std::endl;
    
    auto k1 = Key("1");
    auto k12 = Key("12");
    std::cout<<k1<<std::endl;
    assert(k1=="1");
    assert(k1!=k12);
    assert(k1<k12);
	
    return 0;
}
