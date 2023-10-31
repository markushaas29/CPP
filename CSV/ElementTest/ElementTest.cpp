#include <iostream>
#include <cassert>
#include "../Element.hpp"
#include "../Elements.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	auto s = std::string("123");
    std::cout<<s<<std::endl;

	//~ auto e = Element(s.c_str());
	//~ assert(s.size()==e.Value().size());
	//~ assert(s==e.Value());
    //~ std::cout<<e.Value().size()<<std::endl;
    //~ std::cout<<*(e.End()-2)<<std::endl;
	
	auto s1 = std::string("DE123DE");
	auto s2 = std::string("DE05100110012620778704");
	auto i = IBAN(s1.c_str());
	auto i2 = IBAN(s2.c_str());
	auto i3 = IBAN("");
    std::cout<<i.Get()<<std::endl;
	assert(!i.Valid());
	assert(!i3.Valid());
	assert(i2.Valid());

	std::unique_ptr<IElement> pi = std::make_unique<IBAN>(s2.c_str());
	std::unique_ptr<IElement> pn = std::make_unique<Name>(s1.c_str());
	std::vector<std::unique_ptr<IElement>> v;
	v.push_back(std::move(pi));
	v.push_back(std::move(pn));

	for(auto& p : v)
		std::cout<<"P: "<<p->Get()<<std::endl;
	
    return 0;
}
