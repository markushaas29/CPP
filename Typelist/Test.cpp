#include <iostream>
#include "Typelist.hpp"
#include "../Unit/Unit.hpp"

template<typename Types>
void Parse()
{
	if(Types::Empty)
		return;
	
	using Head = Front<Types>;
	
	if(	Head::Value == 3) 
		std::cout<<Head::Value;
	
	using Tail = PopFront<Types>;
	Parse<Tail>();
}


int main()
{       
    std::cout<<"START"<<std::endl;

	using tl = Typelist<Mass, Sum>::Type; 
// 	using t = tl::Type; 
	using f = Front<tl>::Type; 
    std::cout<<"Empty: "<<tl::Empty<<"Front size: "<<tl::Size<<" Name "<<f::Mass::N<<std::endl;
	
	using tlB = PushBack<tl,Temperature>::Type; 
	using a0 = At<tl,0>::Type; 
	using a1 = At<tl,1>::Type; 
	using a2 = At<tlB,2>::Type; 
    std::cout<<"Empty: "<<tl::Empty<<"0: "<<a0::Mass::N<<" 1: "<<a1::Sum::N<<std::endl;
	
	using t1 = PopFront<tl>::Type;
	using f2 = Front<t1>::Type; 
    std::cout<<"Empty: "<<t1::Empty<<"Front size: "<<t1::Size<<" Name "<<f2::Mass::N<<std::endl;

	using t2 = PopFront<t1>::Type;
    std::cout<<"Empty: "<<t2::Empty<<"Front size: "<<t2::Size<<std::endl;
	
	using tInt = Typelist<T::int_<1>, T::int_<2>, T::int_<3>>::Type; 
    std::cout<<"tInt Front: "<<Front<tInt>::Value<<std::endl;
    
// 	Parse<tInt>();
	
	std::cout<<"END"<<std::endl;

    return 0;
}
