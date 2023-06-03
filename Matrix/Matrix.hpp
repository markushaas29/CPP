#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "Matrix_Ref.hpp"
#include "Matrix_Slice.hpp"
#include "Matrix_Initializer.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixCell.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<std::size_t N, typename T = std::shared_ptr<int>>
//template<std::size_t N, typename T = MatrixCell>
class Matrix
{
public:
	static constexpr size_t Order = N;
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = T;
	using Iterator = typename std::vector<T>::iterator;
	using ConstIterator = typename std::vector<T>::const_iterator;
	//template<typename T1, std::size_t N1> using MatrixInitializer = typename MatrixInit<T1,N1>::Type;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	Matrix(const Matrix&) = default;
	Matrix& operator=(Matrix&) = default;
	~Matrix() = default;

	template<typename U> Matrix(const MatrixRef<N,U>&);
	template<typename U> Matrix& operator=(const MatrixRef<N,U>&);

	template<typename... Exts> explicit Matrix(Exts... exts): descriptor{exts...}, elements{std::make_unique(descriptor.size())} {};

	Matrix(MatrixInitializer<T,N> init)
	{
		descriptor.extents = MI::derive_extents(init);
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.size);
		MI::insert_flat(init,elements);
		for(auto& i : *elements)
			std::cout<<i<<"\t";
		std::cout<<"SIZE: "<<elements->size()<<std::endl;
		std::cout<<"EX: "<<std::endl;
		for(auto i : descriptor.extents)
			std::cout<<i<<"\t";
		std::cout<<"SIZE: "<<elements->size()<<std::endl;
		std::cout<<"STrides: "<<std::endl;
		for(auto i : descriptor.strides)
			std::cout<<i<<"\t";
		std::cout<<std::endl;
	};
	Matrix& operator=(MatrixInitializer<T,N>) {};

	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) Rows() { return descriptor.extents[0]; }
	decltype(auto) Columns() { return descriptor.strides[0]; }
	decltype(auto) operator()(size_t r, size_t c) 
	{
		auto i = r * descriptor.strides[0] + c * descriptor.strides[1];
		return elements->at(i); 
	}
	size_t Extent(size_t n) const { return descriptor.extents[n]; }
	size_t Size() const { return elements.size(); }
	const MatrixSlice<N>& Descriptor() const { return descriptor; }

	decltype(auto) Data() { return elements->data(); }
	//template<typename... Args> 

	MatrixRef<N-1,T> operator[](size_t i);// { return Row(i); }
	MatrixRef<N-1,const T> operator[](size_t i) const;// { return Row(i); }
	MatrixRef<N-1, T> Row(size_t i)
    {  
    	assert(i<Rows());
    	MatrixSlice<N-1> row;
    	//MI::slice_dim<0>(i,descriptor,row);
    	return {row,Data()};
    }
//	MatrixRef<N-1,const T> Row(size_t i) const; //{  }
//	MatrixRef<N-1,T> Col(size_t i); //{ }
//	MatrixRef<N-1, const T> Col(size_t i) const; //{  }
private:
	using MI = MatrixImpl<N>;
	template<typename U, bool B> using IsT =  Is<U,LiteralType,B>;
	MatrixSlice<N> descriptor;
	std::unique_ptr<std::vector<T>> elements = std::make_unique<std::vector<T>>();
};

//template<typename T>
//T& Matrix<0,T>::Row(size_t i) = delete;
//
//template<typename T>
//T& Matrix<1,T>::Row(size_t i) 
//{  
//	return elements->at(1);
//}
//
//template<typename T, size_t N>
//MatrixRef<N-1,T> Matrix<N,T>::Row(size_t i) 
//{  
//	assert(i<rows());
//	MatrixSlice<N-1> row;
//	MatrixImpl<N>::slice_dim<0>(i.descriptor,row);
//	return {row,data()};
//}
