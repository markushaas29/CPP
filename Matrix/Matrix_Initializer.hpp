#include <vector>
#include <initializer_list> 

#pragma once

template<typename T, std::size_t N>
struct MatrixInit 
{
	using Type = std::initializer_list<typename MatrixInit<T,N-1>::Type>;
	//using Type = std::initializer_list<int>;
};

template<typename T>
struct MatrixInit<T,1> 
{
	using Type = std::initializer_list<T>;
};

template<typename T>
struct MatrixInit<T,0> {  }; 

template<typename T, size_t N>
using MatrixInitializer = typename MatrixInit<T,N>::Type;
//{
//public:
//	using Type
//private:
//};
