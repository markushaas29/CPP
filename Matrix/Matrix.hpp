#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "Matrix_Ref.hpp"
#include "MatrixDescriptor.hpp"
#include "Matrix_Initializer.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixElement.hpp"
#include "MatrixCalculator.hpp"
#include "MatrixIO.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../Calculator/Operations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<std::size_t N, typename DT=MatrixDescriptor<N,int,int>>
class Matrix
{
public:
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = N;
	using DescriptorType = DT;
	using Type = Matrix<N,DT>;
	using ParserType = typename DescriptorType::ParserType;
	using IType = typename DT::IType;
	using OType = typename DescriptorType::OType;
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
	Matrix(MatrixInitializer<IType,N> init)
	{
		descriptor.SetExtents(MI::derive_extents(init));
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.Size());
		MI::insert_flat(init,elements);
	};

	Matrix& operator=(Matrix& m) { return Matrix(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}
	Matrix& operator=(MatrixInitializer<IType,N>) {};
	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) operator() (auto... I) const
	{
		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
		auto i = MI::computePosition(descriptor.Extents(),descriptor.Strides(), I...);
		return elements->at(i); 
	}

	decltype(auto) Rows() const { return descriptor.Rows(); }
	decltype(auto) Cols() const { return descriptor.Cols(); }
	size_t Extent(size_t n) const { return descriptor.Extents()[n]; }
	size_t Size() const { return descriptor.Size(); }
	const DescriptorType& Descriptor() const { return descriptor; }

	decltype(auto) operator[] (size_t i) const 
	{
		using MDT = MatrixDescriptor<N-1, IType, OType>;
		std::array<size_t,N-1> e;
		std::array<size_t,N-1> s;
		std::copy(descriptor.Extents().begin()+1, descriptor.Extents().end(), e.begin());
		std::copy(descriptor.Strides().begin()+1, descriptor.Strides().end(), s.begin());
		auto row = Row(i);
		if constexpr ((N-1)==0)
			return MatrixElement<IType>(*(elements->at(i)));
		else
			return Matrix<N-1,MDT>(MDT{e,s}, row);
	}

	decltype(auto) AddRow(const std::vector<IType>& v)
	{
		std::for_each(v.cbegin(), v.cend(), [&](auto i) { elements->push_back(std::make_shared<IType>(i)); } );
		IsT<Throwing>(Format("Not jagged: Size: ",v.size()))(!MI::checkJagged(v.size(),descriptor));
		descriptor.AddRow();
	}
	decltype(auto) Row(size_t i) const
    {  
    	assert(i<Rows());
		std::vector<DataType> result;
		for(auto r = i * Cols(); r < (i+1) * Cols(); r++)
			result.push_back(elements->at(r));
		return result;
    }
	decltype(auto) Col(size_t i)
    {  
    	assert(i<Cols());
		std::vector<DataType> result;
		for(auto r = 0; r < Rows(); r++)
			result.push_back(elements->at(i + (r * Cols())));
		return result;
    }

	template<typename F>
	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
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
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) //{ return (*m.io)(s); }
	{
		if constexpr (Matrix::Order==1)
		{
			std::for_each(m.elements->cbegin(), m.elements->cend(), [&](auto& v) { s<<*v<<", "; });
			return s;
		}
		else
		{
			for(auto i = 0; i != m.Rows(); ++i)
				s<<"{"<<m[i]<<"}"<<std::endl;
		}
		return s;
	}
	template<typename,typename> friend class MatrixCalculator;
	template<template<typename, typename> class T, uint, typename, typename> friend class MatrixCalculatorBase;
	friend class MatrixIO<Type>;
	DescriptorType descriptor;
	ParserType parser;
	//std::unique_ptr<MatrixIO<Type>> io = std::make_unique<MatrixIO<Type>>(this);
	std::unique_ptr<std::vector<DataType>> elements = std::make_unique<std::vector<DataType>>();
};
