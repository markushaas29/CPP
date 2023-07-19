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
#include "../Calculator/CalculatorResult.hpp"
#include "../Calculator/Operations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<std::size_t N, typename DT=MatrixDescriptor<N,int,int,int>>
class Matrix
{
public:
	using DescriptorType = DT;
	using Type = Matrix<N,DT>;
	template<typename T> using MC = MatrixCalculator<Type, T>;
	template<typename,typename> friend class MatrixCalculator;
	template<template<typename, typename> class T, uint, typename, typename> friend class MatrixCalculatorBase;
	using ParserType = typename DescriptorType::ParserType;
	using InputType = typename DT::InputType;
	using ExpressionType = typename DT::ExpressionType;
	using DataType = typename DT::DataType;
	using ExpDataType = typename DT::ExpDataType;
	using OutputTypes = typename DescriptorType::OutputTypes;
	static constexpr size_t Order = N;
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = InputType;
	using Iterator = typename std::vector<ValueType>::iterator;
	using ConstIterator = typename std::vector<ValueType>::const_iterator;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	Matrix(const Matrix& m): descriptor(m.descriptor), elements{std::make_unique<std::vector<DataType>>(m.elements->cbegin(),m.elements->cend())}, expressions{std::make_unique<std::vector<ExpDataType>>(m.expressions->cbegin(),m.expressions->cend())}{ };
	Matrix& operator=(Matrix& m) { return Matrix(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}
	~Matrix() = default;

	explicit Matrix(DescriptorType d, const std::vector<DataType>& v): descriptor(d), elements{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}, expressions{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}{ };
	explicit Matrix(DescriptorType d, const std::vector<DataType>& v, const std::vector<ExpDataType>& e): descriptor(d), elements{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}, expressions{std::make_unique<std::vector<ExpDataType>>(e.begin(),e.end())}{ };
	Matrix(MatrixInitializer<InputType,N> init)
	{
		descriptor.SetExtents(MI::derive_extents(init));
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.Size());
		MI::insert_flat(init,elements);
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& v) { expressions->push_back(std::make_shared<ExpressionType>(*v)); });
	};
	Matrix& operator=(MatrixInitializer<InputType,N>) {};

	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	decltype(auto) Rows() const { return descriptor.Rows(); }
	decltype(auto) Cols() const { return descriptor.Cols(); }
	decltype(auto) operator() (auto... I) const
	{
		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
		auto i = MI::computePosition(descriptor.Extents(),descriptor.Strides(), I...);
		return elements->at(i); 
	}
	size_t Extent(size_t n) const { return descriptor.Extents()[n]; }
	size_t Size() const { return descriptor.Size(); }
	const DescriptorType& Descriptor() const { return descriptor; }

	decltype(auto) Data() { return elements->data(); }
	decltype(auto) operator[] (size_t i) const 
	{
		using MDT = MatrixDescriptor<N-1, InputType, ExpressionType, OutputTypes>;
		std::array<size_t,N-1> e;
		std::array<size_t,N-1> s;
		std::copy(descriptor.Extents().begin()+1, descriptor.Extents().end(), e.begin());
		std::copy(descriptor.Strides().begin()+1, descriptor.Strides().end(), s.begin());
		auto row = Row(i);
		auto expRow = ExpRow(i);
		if constexpr ((N-1)==0)
		{
			return ElementAt(i);
		}
		else
			return Matrix<N-1,MDT>(MDT{e,s}, row, expRow);
	}

	decltype(auto) AddRow(const std::vector<InputType>& v)
	{
		std::for_each(v.cbegin(), v.cend(), [&](auto i) { elements->push_back(std::make_shared<InputType>(i)); } );
		std::for_each(v.cbegin(), v.cend(), [&](auto i) { expressions->push_back(std::make_shared<InputType>(i)); } );
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
	
	decltype(auto) ExpRow(size_t i) const
    {  
    	assert(i<Rows());
		std::vector<ExpDataType> result;
		for(auto r = i * Cols(); r < (i+1) * Cols(); r++)
			result.push_back(expressions->at(r));
		return result;
    }
	
	decltype(auto) ElementsAt(size_t i) const
    {
		auto r = Row(i);
		return parser.Parse(r);
    }
	decltype(auto) ElementAt(size_t n, size_t m = 0) const {	return ElementsAt(n)[m]; }

	decltype(auto) Col(size_t i)
    {  
    	assert(i<Cols());
		std::vector<DataType> result;
		for(auto r = 0; r < Rows(); r++)
			result.push_back(elements->at(i + (r * Cols())));
		return result;
    }

	template<typename F>
	decltype(auto) Apply(F f)
	{
		auto el = std::vector<DataType>();
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& e) { el.push_back(std::make_shared<InputType>(f(e))); });
		return Type(descriptor,el); 
	}

  	decltype(auto) operator+(const auto& v)	{ return MC<Type>::apply(*this,Add<InputType,decltype(v)>{v});  }
  	decltype(auto) operator-(const auto& v)	{ return MC<Type>::apply(*this,Sub<InputType,decltype(v)>{v});  	}
  	decltype(auto) operator*(const auto& v)	{ return MC<Type>::apply(*this,Mul<InputType,decltype(v)>{v});  	}
  	decltype(auto) operator/(const auto& v)	{ return MC<Type>::apply(*this,Div<InputType,decltype(v)>{v});   	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator+(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::add(*this,m);  	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator-(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::sub(*this,m);  	}
  	template<size_t N2, typename D2>
	decltype(auto) operator*(const Matrix<N2, D2>& m)	{ return MC<Matrix<N2,D2>>::multiply(*this,m);  	}
  	decltype(auto) operator/(const Type& m)	{ return apply(std::vector<DataType>(m.elements->cbegin(), m.elements->cend()), [&](const auto& e1, const auto& e2){ return *e1 / *e2; });  	}
private:
	template<size_t N2, typename D2> friend class Matrix;
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) 
	{
		if constexpr (Matrix::Order==1)
		{
			std::for_each(m.elements->cbegin(), m.elements->cend(), [&](auto& v) { s<<*v<<", "; });
			s<<"\nExpressions:\n";
			std::for_each(m.expressions->cbegin(), m.expressions->cend(), [&](auto& v) { s<<*v<<", "; });
			return s;
		}
		else
		{
			for(auto i = 0; i != m.Rows(); ++i)
				s<<"{"<<m[i]<<"}"<<std::endl;
		}
		return s;
	}

	template<typename F>
	decltype(auto) apply(const auto& m, F f)
	{
		auto el = std::vector<DataType>();
		for(auto i = 0; i < elements->size(); ++i)
			el.push_back(std::make_shared<InputType>(f(m.at(i),elements->at(i))));
		return Type(descriptor,el); 
	}
	using MI = MatrixImpl<N,DescriptorType>;
	template<typename U> using IsT =  Is<U,LiteralType>;
	DescriptorType descriptor;
	ParserType parser;
	std::unique_ptr<std::vector<DataType>> elements = std::make_unique<std::vector<DataType>>();
	std::unique_ptr<std::vector<ExpDataType>> expressions = std::make_unique<std::vector<ExpDataType>>();
};
