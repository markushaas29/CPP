#include <initializer_list>
#include <array>
#include <cassert>
#include <algorithm>
#include "Matrix_Ref.hpp"   
#include "MatrixDescriptor.hpp"   
#include "MatrixAccess.hpp"   
#include "MatrixInit.hpp"   
#include "../Is/Is.hpp"   
#include "../String/Format.hpp"   

#pragma once

template<size_t, typename> class Matrix;


template<size_t N, typename DescriptorType>
class MatrixImpl
{
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixImpl";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	template<uint I, uint IA=I-1> 
	static decltype(auto) checkExtents(const std::array<std::size_t, N> extents)
	{
		if constexpr (I==N)
			return extents;
		else
		{
			if(std::all_of(extents.begin(), extents.end(), [](auto i){ return i==0;})) 
				return extents;
			IsT<Throwing>(Format(": Extent is zero at Dimension: ",I))(extents[I]>0);
			return  checkExtents<I+1>(extents);
		}
	}
private:
	template<typename T> using IsT = Is<T,LiteralType>;
	template<size_t,typename> friend class Matrix;
	template<typename> friend class MatrixAccess;
	friend DescriptorType;
	
	template<typename List> 
	static decltype(auto) derive_extents(const List& list) 
	{
		std::array<std::size_t, N> a;
		auto f = a.begin();
		add_extents<N>(f,list);
		return a;
	};   

	static void compute_strides(DescriptorType& ms)
	{
		size_t st = 1;
		for(int i = N-1; i>= 0; --i)
		{
			ms.strides[i] = st;
			st *= ms.extents[i];
		}
	}

	static constexpr decltype(auto) checkJagged(size_t s, const DescriptorType& d)	{	return s % d.strides[0];	}

	template<uint I> 
	static decltype(auto) computeIndex(const std::array<std::size_t, N> indices, const std::array<std::size_t, N> extents, const std::array<std::size_t, N> strides)
	{
		if constexpr (I==N)
			return 0;
		else
		{
			IsT<Throwing>(Format("Index exceeds extents: Index: ",indices[I]," >= Extent: ",extents[I]))(indices[I]<extents[I]);
			return indices[I] * strides[I] + computeIndex<I+1>(indices,extents,strides);
		}
	}
	
	static decltype(auto) computePosition(const std::array<std::size_t, N> extents, const std::array<std::size_t, N> strides, auto... Is)
	{
		auto Dim = sizeof...(Is);
		IsT<Throwing>("Dim != N")(Dim==N);
 	    std::initializer_list<int> il({Is...} );
		std::array<size_t, N> indices;
        std::copy (il.begin(), il.end(), indices.begin());
		
		return computeIndex<0>(indices,extents,strides);
	}
	template<typename T, typename V> 
	static void insert_flat(std::initializer_list<T> list, V& v){	add_list(list.begin(), list.end(),v);	}

	template<size_t N1,typename I, typename List> 
	static typename std::enable_if<(N1>1),void>::type add_extents(I& first, const List& list)
	{
		//assert(check_non_jagged<N>(list));
		*first++ = list.size();
		add_extents<N1-1>(first,*list.begin());
	}
	
	template<size_t N1,typename I, typename List> 
	static typename std::enable_if<N1==1,void>::type add_extents(I& first, const List& list){	*first++ = list.size();	}

	template<typename T, typename V> 
	static void add_list(const std::initializer_list<T> list, V& v)	{	add_list(list.begin(),list.end(),v);	}
	
	template<typename T, typename V> 
	static void add_list(const std::initializer_list<T>* first, const std::initializer_list<T>* last,V& v)
	{
		for(;first !=last; ++first)
			add_list(first->begin(), first->end(),v);
	}

	template<typename T, typename V> 
	static void add_list(const T* first, const T* last,V& v)
	{	
		std::for_each(first,last,[&](auto e) { v->push_back(std::make_shared<T>(e)); });	
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

	template<std::size_t N1,typename... Dims>
	bool check_bounds(const DescriptorType& s, Dims... d)
	{
		size_t indexes[N] { size_t(d)... };
		return equal(indexes,indexes+N, s.extents.begin(), std::less<size_t>{});
	}

	template<typename... Args>
	constexpr bool Requesting_element()	{	return All(std::is_convertible<Args,size_t>{}...);	}
	
//	template<typename... Args>
//	constexpr bool Requesting_slice()
//	{
//		return All((std::is_convertible<Args,size_t>{} || std::is_same<Args, slice>{})...);
//	}
};
