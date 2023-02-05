#include <iostream>
#include "Traits.hpp"

int main()
{
	std::cout<<"Test start"<<std::endl;
	
	std::cout<<"Ref: int->"<<Traits::ReferenceTraits<int>::Result<<std::endl;
	std::cout<<"Ref: int&->"<<Traits::ReferenceTraits<int&>::Result<<std::endl;
	std::cout<<"Ref: int*->"<<Traits::ReferenceTraits<int*>::Result<<std::endl;
	
	int i = 5;
	int& j = i;
	
	std::cout<<"i ->"<<Traits::ReferenceTraits<int>::RemoveRef(i)<<std::endl;
	std::cout<<"j ->"<<Traits::ReferenceTraits<int&>::RemoveRef(j)<<std::endl;
	
	
	std::cout<<"Test end";
	
	return 0;
}
