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
class Analyzer<Counter<C> ,A>
{
public:
	using Type = Counter<C>;
	static decltype(auto) Get()
	{
		Type::readings;
		std::cout<<"Counter"<<std::endl;
	};
};
namespace Bank{ template<typename ConfigT> class Account; }

template<typename T, typename A>
class Analyzer<Bank::Account<T> ,A>
{
public:
	using Type = Bank::Account<T>;
	static decltype(auto) Get()
	{
		T::cont;
		std::cout<<"Account"<<std::endl;
	};
};
	
