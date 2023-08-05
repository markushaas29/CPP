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

template<std::size_t N, typename DT=MatrixDescriptor<N,int>>
class Matrix
{
public:
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = N;
	using DescriptorType = DT;
	using Type = Matrix<N,DT>;
	using IType = typename DT::IType;
	using DataType = typename DT::DataType;
	using ValueType = IType;
	using MI = MatrixImpl<N,DescriptorType>;
	template<typename T> using MC = MatrixCalculator<Type, T>;
	template<typename U> using IsT =  Is<U,LiteralType>;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	~Matrix() = default;

	Matrix(const Matrix& m): descriptor(m.descriptor), elements{std::make_unique<std::vector<DataType>>(m.elements->cbegin(),m.elements->cend())} { };
	explicit Matrix(DescriptorType d, const std::vector<DataType>& v): descriptor(d), elements{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}{ };
	Matrix(MatrixInit<IType,N> init)
	{
		descriptor.SetExtents(MI::derive_extents(init));
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.Size());
		MI::insert_flat(init,elements);
	};

	Matrix& operator=(Matrix& m) { return Matrix(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}
	Matrix& operator=(MatrixInit<IType,N>) {};
	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) Rows() const { return descriptor.Rows(); }
	decltype(auto) Cols() const { return descriptor.Cols(); }
	size_t Extent(size_t n) const { return descriptor.Extents()[n]; }
	size_t Size() const { return descriptor.Size(); }
	const DescriptorType& Descriptor() const { return descriptor; }

	decltype(auto) operator[] (size_t i) const { return access->matrix(i,this); }
	decltype(auto) AddRow(const std::vector<IType>& v) { access->addRow(v,this); }
	decltype(auto) Row(size_t i) const { return access->row(i, this); }
	decltype(auto) Col(size_t i) const { return access->col(i, this); }

	template<typename F>
	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
	decltype(auto) ColSum() { return MC<Type>::colSum(*this); }
	decltype(auto) RowSum() { return MC<Type>::rowSum(*this); }
  	decltype(auto) operator+(const auto& v)	{ return MC<Type>::apply(*this,Add<IType,decltype(v)>{v});  }
  	decltype(auto) operator-(const auto& v)	{ return MC<Type>::apply(*this,Sub<IType,decltype(v)>{v});  	}
  	decltype(auto) operator*(const auto& v)	{ return MC<Type>::apply(*this,Mul<IType,decltype(v)>{v});  	}
  	decltype(auto) operator/(const auto& v)	{ return MC<Type>::apply(*this,Div<IType,decltype(v)>{v});   	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator+(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::add(*this,m);  	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator-(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::sub(*this,m);  	}
  	template<size_t N2, typename D2>
	decltype(auto) operator*(const Matrix<N2, D2>& m)	{ return MC<Matrix<N2,D2>>::multiply(*this,m);  	}
  	decltype(auto) operator/(const Type& m)	{ return   Type(descriptor,std::vector<DataType>(elements->cbegin(), elements->cend()));	}
private:
	decltype(auto) operator() (auto... I) const
	{
		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
		auto i = MI::computePosition(descriptor.Extents(),descriptor.Strides(), I...);
		return elements->at(i); 
	}

	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) { return (*m.io)(s,&m); }
	template<typename,typename> friend class MatrixCalculator;
	template<template<typename, typename> class T, uint, typename, typename> friend class MatrixCalculatorBase;
	friend class MatrixAccess<Type>;
	friend class MatrixIO<Type>;

	DescriptorType descriptor;
	std::unique_ptr<MatrixAccess<Type>> access = std::make_unique<MatrixAccess<Type>>();
	std::unique_ptr<MatrixIO<Type>> io = std::make_unique<MatrixIO<Type>>();
	std::unique_ptr<std::vector<DataType>> elements = std::make_unique<std::vector<DataType>>();
};
