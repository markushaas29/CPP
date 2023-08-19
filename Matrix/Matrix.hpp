#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "Matrix_Ref.hpp"
#include "MatrixDescriptor.hpp"
#include "MatrixInit.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixElement.hpp"
#include "MatrixCalculator.hpp"
#include "MatrixAccess.hpp"
#include "MatrixIO.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../Calculator/Operations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename> class MatrixFilter;

template<std::size_t N, typename DT=MatrixDescriptor<N,int>>
class Matrix
{
public:
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = N;
	using Type = Matrix<N,DT>;
	using DescriptorType = DT;
	using ElementType = typename DT::ElementType;
	using DataType = typename DT::DataType;
	using ValueType = ElementType;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	~Matrix() = default;

	Matrix(const Matrix& m): descriptor(m.descriptor), elements{std::make_unique<std::vector<DataType>>(m.elements->cbegin(),m.elements->cend())} { check();};
	explicit Matrix(DescriptorType d, const std::vector<DataType>& v): descriptor(d), elements{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}{ check(); };
	Matrix(MatrixInit<ElementType,N> init)
	{
		descriptor.SetExtents(MI::derive_extents(init));
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.Size());
		MI::insert_flat(init,elements);
	};

	Matrix& operator=(Matrix& m) { return Matrix(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}
	Matrix& operator=(MatrixInit<ElementType,N>) {};
	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) Rows() const { return descriptor.Rows(); }
	decltype(auto) Cols() const { return descriptor.Cols(); }
	size_t Extent(size_t n) const { return descriptor.Extents()[n]; }
	size_t Size() const { return descriptor.Size(); }
	const DescriptorType& Descriptor() const { return descriptor; }

	decltype(auto) operator[] (size_t i) const { return access->matrix(i,this); }
	decltype(auto) AddRow(const std::vector<ElementType>& v) { access->addRow(v,this); }
	decltype(auto) Slice(size_t i) const { return access->slice(i, this); }
	//template<size_t... S>
	decltype(auto) Slices(auto... i) const { return access->slices(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }

	template<typename F>
	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
	decltype(auto) ColSum() { return MC<Type>::colSum(*this); }
	decltype(auto) RowSum() { return MC<Type>::rowSum(*this); }
  	decltype(auto) operator+(const auto& v)	{ return MC<Type>::apply(*this,Add<ElementType,decltype(v)>{v});  }
  	decltype(auto) operator-(const auto& v)	{ return MC<Type>::apply(*this,Sub<ElementType,decltype(v)>{v});  	}
  	decltype(auto) operator*(const auto& v)	{ return MC<Type>::apply(*this,Mul<ElementType,decltype(v)>{v});  	}
  	decltype(auto) operator/(const auto& v)	{ return MC<Type>::apply(*this,Div<ElementType,decltype(v)>{v});   	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator+(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::add(*this,m);  	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator-(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::sub(*this,m);  	}
  	template<size_t N2, typename D2>
	decltype(auto) operator*(const Matrix<N2, D2>& m)	{ return MC<Matrix<N2,D2>>::multiply(*this,m);  	}
  	decltype(auto) operator/(const Type& m)	{ return   Type(descriptor,std::vector<DataType>(elements->cbegin(), elements->cend()));	}
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<typename T> using MC = MatrixCalculator<Type, T>;
	using MI = MatrixImpl<N,DescriptorType>;
	
	decltype(auto) operator() (auto... I) const
	{
		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
		auto i = MI::computePosition(descriptor.Extents(),descriptor.Strides(), I...);
		return elements->at(i); 
	}
	
	decltype(auto) row(size_t i) const { return access->row(i, this); }
	decltype(auto) col(size_t i) const { return access->col(i, this); }
	decltype(auto) check() const {	IsT<Throwing>(Format("Matrix is jagged Rows/Cols: ",Rows(), "/", Cols(), " Size: ", elements->size()))((Rows()*descriptor.Stride(0))==elements->size());	}

	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) { return (*m.io)(s,&m); }
	template<typename,typename> friend class MatrixCalculator;
	template<template<typename, typename> class T, uint, typename, typename> friend class MatrixCalculatorBase;
	friend class MatrixAccess<Type>;
	friend class MatrixIO<Type>;
	friend class MatrixFilter<Type>;

	DescriptorType descriptor;
	std::unique_ptr<MatrixAccess<Type>> access = std::make_unique<MatrixAccess<Type>>();
	std::unique_ptr<MatrixIO<Type>> io = std::make_unique<MatrixIO<Type>>();
	std::unique_ptr<std::vector<DataType>> elements = std::make_unique<std::vector<DataType>>();
};
