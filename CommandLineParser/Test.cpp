#include <iostream>
#include <stdio.h>
#include "Parser.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
// 	std::string s;
// 	std::getline(std::cin,s);
// 	
// 	for(auto s_ : s)
// 		std::cout<<(int)s_<<std::endl;
		
	
	char c;
	auto p = CommandLine::Parser();
// 	while(c = std::cin.get())
// 		p.Event(c);
// 	
	p.Parse();
// 	std::string s = "END";
// 	std::string a;
//     std::cout<<s;
// 	std::cin>>a;
// 
//     std::cout<<(s+a)<<std::endl;

	std::cout<<"END"<<std::endl;
	std::cout<<"END2"<<std::endl;

    return 0;
}
