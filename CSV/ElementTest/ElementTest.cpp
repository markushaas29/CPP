#include <iostream>
#include <cassert>
#include "../CSV.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	auto s = std::string("123");
    std::cout<<s<<std::endl;

	auto e = Element(s.c_str());
	assert(s.size()==e.Value().size());
	assert(s==e.Value());
    std::cout<<e.Value().size()<<std::endl;
    std::cout<<*(e.End()-2)<<std::endl;
	
	auto s1 = std::string("DE123DE");
	auto i = IBAN(s1.c_str());
	assert(s1.size()==i.Value().size());
	assert(s1==i.Value());
    std::cout<<i.Value()<<std::endl;
	
    std::cout<<"Num2String"<<std::endl;
    std::cout<<"9: "<<NumToChar(9)<<std::endl;
    std::cout<<"0: "<<NumToChar(0)<<std::endl;
	
    std::cout<<"Int2Chars"<<std::endl;
    std::cout<<"12: "<<std::string(IntToChars<2>(12).data())<<std::endl;
    std::cout<<"20: "<<std::string(IntToChars<2>(20).data())<<std::endl;
    std::cout<<"20: "<<IntToChars<2>(20)[0]<<std::endl;
    std::cout<<"20: "<<IntToChars<2>(20)[1]<<std::endl;
    std::cout<<"21: "<<std::string(IntToChars<2>(21).data())<<std::endl;
    std::cout<<"31: "<<std::string(IntToChars<2>(31).data())<<std::endl;
    std::cout<<"89: "<<std::string(IntToChars<2>(89).data())<<std::endl;
    
    std::cout<<"Num2String"<<std::endl;
    std::cout<<num_to_string<4568>::value<<std::endl;
	
    return 0;
}
