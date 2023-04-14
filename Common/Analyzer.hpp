#pragma once

template<typename T>
concept CounterConcept = requires(T t) 
{
	T::readings;
}; 

template<typename T, typename A> class Analyzer {
public:
	static decltype(auto) Get()
	{
		std::cout<<"\nT"<<std::endl;
	};
};
//template<typename ConfigT> class Counter;
template<CounterConcept C, typename A>
class Analyzer<C,A>
{
public:
	static decltype(auto) Get()
	{
		std::cout<<"Counter"<<std::endl;
	};
};

	
	
