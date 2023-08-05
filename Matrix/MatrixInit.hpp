#include <vector>
#include <initializer_list> 

#pragma once

template<typename T, std::size_t N>
struct Matrix_Init 
{
	using Type = std::initializer_list<typename Matrix_Init<T,N-1>::Type>;
	//using Type = std::initializer_list<int>;
};

template<typename T>
struct Matrix_Init<T,1> 
{
	using Type = std::initializer_list<T>;
};

template<typename T>
struct Matrix_Init<T,0>
{
	using Type = T;
};

template<typename T, size_t N>
using MatrixInit = typename Matrix_Init<T,N>::Type;
