#include <vector>
#include <cassert>
#include "MatrixDescriptor.hpp"

#pragma once

template< size_t N, typename T>
class MatrixRef
{
public:
	MatrixRef(const MatrixDescriptor<N>& s, T* p): descriptor{s}, ptr{p} {}
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixRef& i) { return s<<i.descriptor;  }
	MatrixDescriptor<N> descriptor;
	T* ptr;
};
