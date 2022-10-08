#include <iostream>
#include <vector>
#include "../Logger/Logger.hpp"
#include "Algorithms.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
    using T = Algorithms::Equal<int>::Type; 
    T t1 = std::make_shared<int>(1);
    T t2 = std::make_shared<int>(2);
    T t3 = std::make_shared<int>(3);
	
	auto e1 = Algorithms::Equal<int>(t1);
	auto e2 = Algorithms::Equal<int>(t2);
	auto ne2 = Algorithms::NotEqual<int>(t2);
	
	std::cout<<"e1 "<<e1(t1)<<std::endl;
	std::cout<<"e2 "<<e2(t1)<<std::endl;
	
	std::cout<<"ne2 "<<ne2(t1)<<std::endl;
	std::cout<<"ne2 "<<ne2(t2)<<std::endl;
	
	std::vector<T> ts = {t1, t2, t3};
	std::vector<T> result;
	result.reserve(3);
	
	auto r = Algorithms::copy_if(ts.cbegin(), ts.cend(), result.begin(), ne2);
	
	std::cout<<*r<<std::endl;
	
	return 0;
}
