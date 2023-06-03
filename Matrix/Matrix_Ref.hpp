#include <vector>
#include <cassert>
#include "Matrix_Slice.hpp"

#pragma once

template< size_t N, typename T>
class MatrixRef
{
public:
	MatrixRef(const MatrixSlice<N>& s, T* p): descriptor{s}, ptr{p} {}
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixRef& i) { return s<<i.descriptor;  }
	MatrixSlice<N> descriptor;
	T* ptr;
};
