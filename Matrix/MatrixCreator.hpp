#include <memory>
#include "Matrix.hpp" 

#pragma once

template<size_t N, typename> class MatrixImpl;
template<size_t, typename, typename> class MatrixDescriptor;

template<size_t N, typename T, typename OT>
class MatrixCreator
{
public:
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixCreator& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};
