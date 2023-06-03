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

template<std::size_t N, typename BT=int>
class Matrix
{
	using T = std::shared_ptr<BT>;
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
	};
	Matrix& operator=(MatrixInitializer<T,N>) {};

	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) Rows() const { return descriptor.extents[0]; }
	decltype(auto) Cols() const { return descriptor.strides[0]; }
	decltype(auto) operator() (size_t r, size_t c) const
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
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) 
	{
		s<<"Extents: \n";
		std::for_each(m.descriptor.extents.cbegin(), m.descriptor.extents.cend(), [&s](const auto& i) { s<<i<<"\t"; });
		s<<"\nStride: \n";
		std::for_each(m.descriptor.strides.cbegin(), m.descriptor.strides.cend(), [&s](const auto& i) { s<<i<<"\t"; });
		s<<"\n{\n";
		for(auto i = 0; i != m.Rows(); ++i)
		{
			s<<" {";
			for(auto j = 0; j != m.Cols(); ++j)
			{
				s<<*m(i,j);
				if(j+1!=m.Cols())
					s<<", ";
			}
			s<<"}";
			if(i+1!=m.Rows())
				s<<",\n";
		}
		return s<<"\n}";  
	}

	using MI = MatrixImpl<N>;
	template<typename U, bool B> using IsT =  Is<U,LiteralType,B>;
	MatrixSlice<N> descriptor;
	std::unique_ptr<std::vector<T>> elements = std::make_unique<std::vector<T>>();
};

template<typename BT>
class Matrix<1,BT>
{
	using T = std::shared_ptr<BT>;
public:
	static constexpr size_t Order = 1;
	inline static constexpr const char TypeIdentifier[] = "Matrix<1>";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = T;

	//Matrix(std::initializer_list<U> l): element{e} {}
	//Matrix& operator=(const BT& v)
	//{
	//	element = v;
	//	return *this;
	//}

	T& Row(size_t i) 	{ return elements->at(1); }
	decltype(auto) Rows() const { return 1; }
	decltype(auto) Cols() const { return 0; }
	decltype(auto) operator() () const	{	return elements;	}
	size_t Extent(size_t n) const { return elements->size(); }
	size_t Size() const { return elements->size(); }
private:
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) 	{	return s<<*(m.element);	}
	std::unique_ptr<std::vector<T>> elements = std::make_unique<std::vector<T>>();
};

template<typename BT>
class Matrix<0,BT>
{
	using T = std::shared_ptr<BT>;
public:
	static constexpr size_t Order = 0;
	inline static constexpr const char TypeIdentifier[] = "Matrix<0>";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = T;

	Matrix(const T& e): element{e} {}
	Matrix& operator=(const BT& v)
	{
		element = v;
		return *this;
	}

	const BT& Row(size_t i) = delete;
	decltype(auto) Rows() const { return 0; }
	decltype(auto) Cols() const { return 0; }
	decltype(auto) operator() () const	{	return element;	}
	size_t Extent(size_t n) const { return 1; }
	size_t Size() const { return 1; }
private:
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) 	{	return s<<*(m.element);	}
	T element;
};
