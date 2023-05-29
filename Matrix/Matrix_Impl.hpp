#include <array>
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
	
	template<std::size_t N1,typename List> 
	decltype(auto) derive_extents(const List& list) 
	{
		std::array<std::size_t, N1> a;
		auto f = a.begin();
		add_extents<N>(f,list);
		return a;
	};   

	template<int N1>
 	decltype(auto) compute_strides(MatrixSlice<N>& ms) 
	{
		std::size_t st = 1;
		for(int i=N1-1; i>=0; --i)
		{
			ms.strides[i] = st;
			st *= ms.extents[i];
		}
		ms.size = st;
	};
	
	template<std::size_t N1, typename I, typename List> 
	std::enable_if<(N1>1),void> add_extents(I& first, const List& list)
	{
		assert(check_non_jagged<N>(list));
		*first++ = list.size();
		add_extents<N-1>(first,*list.begin());
	}
	
	template<std::size_t N1, typename I, typename List> 
	std::enable_if<(N1==1),void> add_extents(I& first, const List& list)
	{
		*first++ = list.size();
	}


	template<std::size_t N1,typename List> 
	bool check_non_jagged(const List list)
	{
		auto i = list.begin();
		for(auto j = i+1; j!=list.end(); ++i)
			if(derive_extents<N-1>(*i) != derive_extents<N-1>(*j))
				return false;
		return true;
	}
};
