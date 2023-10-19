#include <vector>
#include <initializer_list>
#include <memory>
#include "Matrix_Ref.hpp"
#include "MatrixDescriptor.hpp"
#include "MatrixInit.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixElement.hpp"
#include "MatrixCalculator.hpp"
#include "MatrixAccess.hpp"
#include "MatrixIO.hpp"
#include "IMatrixQuery.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename> class MatrixFilter;

template<typename T>
decltype(auto) ToDataType(const std::vector<T>& v)
{
	std::vector<std::shared_ptr<T>> result;
	std::for_each(v.begin(),v.end(), [&result](const T& t) { result.push_back(std::make_shared<T>(t)); });
	return result;
};

template<std::size_t N, typename DT=MatrixDescriptor<N,int>>
class Matrix
{
public:
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal TypeId{TypeIdentifier};
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
	explicit Matrix(DescriptorType d, const std::vector<ElementType>& v): Matrix(d,ToDataType(v)) {  };
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
	decltype(auto) Col(size_t i) const { return access->colAt(i, this); }
	decltype(auto) Cols(auto... i) const { return access->cols(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
	decltype(auto) Cols(std::array<size_t,2> span) const { return access->sub(std::array<size_t,2>{0, Rows()-1 }, span, this); }
	decltype(auto) Rows(auto... i) const { return access->rows(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
	decltype(auto) Rows(std::array<size_t,2> span) const { return access->sub(span, std::array<size_t,2>{0, Cols()-1 }, this); }
	decltype(auto) M(std::array<size_t,2> rowSpan, std::array<size_t,2> colSpan) const { return access->sub(rowSpan,colSpan, this); }
	template<typename ET = ElementType>
	decltype(auto) M(const IMatrixQuery<Type, ET>& query) { return query(this); }
	decltype(auto) Slices(std::vector<size_t> rows, std::vector<size_t> cols) const { return access->slices(rows,cols, this); }
	template<typename T>
	decltype(auto) To() const { return access->template to<T>(this); }

	template<typename F>
	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
	template<template<typename> class Op>
	decltype(auto) Calc() { return MC<Type>::template calc<Op>(elements->cbegin(), elements->cend()); }
	decltype(auto) ColSum(int i) { return MC<Type>::colSum(*this,i); }
	decltype(auto) ColSum() { return MC<Type>::colSum(*this); }
	decltype(auto) RowSum() { return MC<Type>::rowSum(*this); }
  	decltype(auto) operator+(const auto v)	{ return MC<Type>::apply(*this,ElementAdd<ElementType,decltype(v)>{Constant<decltype(v)>(v)});  }
  	decltype(auto) operator-(const auto v)	{ return MC<Type>::apply(*this,ElementSub<ElementType,decltype(v)>{Constant<decltype(v)>(v)});  	}
  	decltype(auto) operator*(const auto v)	{ return MC<Type>::apply(*this,ElementMul<ElementType,decltype(v)>{Constant<decltype(v)>(v)});  	}
  	decltype(auto) operator/(const auto v)	{ return MC<Type>::apply(*this,ElementDiv<ElementType,decltype(v)>{Constant<decltype(v)>(v)});   	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator+(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::add(*this,m);  	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator-(const Matrix<N2,D2>& m)	{ return MC<Matrix<N2,D2>>::sub(*this,m);  	}
  	template<size_t N2, typename D2>
	decltype(auto) operator*(const Matrix<N2, D2>& m)	{ return MC<Matrix<N2,D2>>::multiply(*this,m); 	}
  	template<size_t N2, typename D2>
  	decltype(auto) operator/(const Matrix<N2, D2>& m)	{ return MC<Matrix<N2,D2>>::divide(*this,m);	}
private:
	template<typename U> using IsT =  Is<U,TypeId>;
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
	template<typename TT, typename ETQ> friend class IMatrixQuery;

	DescriptorType descriptor;
	std::unique_ptr<MatrixAccess<Type>> access = std::make_unique<MatrixAccess<Type>>();
	std::unique_ptr<MatrixIO<Type>> io = std::make_unique<MatrixIO<Type>>();
	std::unique_ptr<std::vector<DataType>> elements = std::make_unique<std::vector<DataType>>();
};
