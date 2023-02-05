#include <iostream>

#ifndef TRAITS_H
#define TRAITS_H

namespace Traits 
{

template<bool Condition, class T = void>
struct enableIf
{
};

template<class T>
struct enableIf<true, T>
{
	using Type = T;
};

template<bool Condition, class T = void>
using EnableIf = typename enableIf<Condition, T>::Type;

template<typename U> 
struct ReferenceTraits	
{
	enum {Result = false};
	
	template<typename T>
	bool IsRef(T t){ std::cout<<"U"<<std::endl;return ReferenceTraits::Result; }
	
	template<typename T>
	static T RemoveRef(T t){ std::cout<<"U"<<std::endl;return t;}
};

template<typename U> 
struct ReferenceTraits<U&>	
{
	enum {Result = true};
	
	template<typename T>
	bool IsRef(T t){ std::cout<<"U&"<<std::endl;return ReferenceTraits<T>::Result; }
	
	template<typename T>
	static T RemoveRef(T t){ std::cout<<"U&"<<std::endl;return t;}
};


template<typename U> 
struct RefTraits	
{		
	static bool IsRef(U u){ return false; }
	
	static bool IsRef(U& u){ return true; }
	/*
	template<typename T>
	static T RemoveRef(T t){ return t;}
	
	template<>
	static T RemoveRef<T&>(T t){ return t;}*/
};

	
}

#endif
