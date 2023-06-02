#include <vector>
#include "Matrix_Slice.hpp"

#pragma once

template< size_t N, typename T>
class MatrixRef
{
public:
	MatrixRef(const MatrixSlice<N>& s, T* p): descriptor{s}, ptr{p} {}
private:
	MatrixSlice<N> descriptor;
	T* ptr;
};
