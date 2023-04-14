#pragma once

template<typename T>
concept CounterConcept = requires(T t) 
{
	T::readings;
}; 

template<typename T, typename A> class Analyzer {
public:
};
template<typename ConfigT> class Counter;

template<typename C, typename A>
class Analyzer<Counter<C>,A>
{
public:
	static decltype(auto) Get()
	{
		std::cout<<"Counter"<<std::endl;
	};
};

	
	
