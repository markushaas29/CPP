#include <vector>

#pragma once

template<size_t N>
class MatrixInitializer 
{
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixInitializer";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = N;
//	using Type = Matrix<N,DT>;
//	using DescriptorType = DT;
//	using ElementType = typename DT::ElementType;
//	using DataType = typename DT::DataType;
//	using ValueType = ElementType;
};

template<typename T>
decltype(auto) Init(std::vector<T> v) { return MatrixInitializer<1>(); }

template<typename T>
decltype(auto) Init(std::vector<std::vector<T>> v) { return MatrixInitializer<2>(); }

template<typename T>
decltype(auto) Init(std::vector<std::vector<std::vector<T>>> v) { return MatrixInitializer<3>(); }
