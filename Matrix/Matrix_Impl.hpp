#include <vector>
#include "Matrix_Ref.hpp"   
#include "Matrix_Slice.hpp"   
#include "Matrix_Initializer.hpp"   

#pragma once

template<size_t, typename> class Matrix;

template<size_t N>
class MatrixImpl
{
public:
private:
	template<size_t,typename> friend class Matrix;
	decltype(auto) derive_extents(auto init) { };    
 	decltype(auto) compute_strides(auto descriptor) { };
};
