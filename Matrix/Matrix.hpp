#include <vector>
#include <initializer_list>
#include <memory>
#include "Matrix_Ref.hpp" 
#include "Matrix_Slice.hpp" 
#include "Matrix_Initializer.hpp" 
#include "Matrix_Impl.hpp" 
#include "MatrixCell.hpp" 
#include "../Is/Is.hpp" 
#include "../String/Literal.hpp" 

#pragma once

template<size_t N, typename T = MatrixCell>
class Matrix
{
public:
	static constexpr size_t Order = N;
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = T;
	using Iterator = typename std::vector<T>::iterator;
	using ConstIterator = typename std::vector<T>::const_iterator;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	Matrix(const Matrix&) = default;
	Matrix& operator=(Matrix&) = default;
	~Matrix() = default;

	template<typename U> Matrix(const MatrixRef<U,N>&);
	template<typename U> Matrix& operator=(const MatrixRef<U,N>&);

	template<typename... Exts> explicit Matrix(Exts... exts): descriptor{exts...}, elements{std::make_unique(descriptor.size())} {};

	Matrix(MatrixInitializer<T,N> init) 
	{
		descriptor.extents = MI::derive_extents(init);
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.size);
	};
	Matrix& operator=(MatrixInitializer<T,N>) {};
	
	template<typename U> Matrix(std::initializer_list<U>) = delete;
	template<typename U> Matrix& operator=(std::initializer_list<U>) = delete;

	size_t Extent(size_t n) const { return descriptor.extents[n]; }
	size_t Size() const { return elements.size(); }
	const MatrixSlice<N>& Descriptor() const { return descriptor; }

	decltype(auto) Data() { return elements.data(); }

private:
	using MI = MatrixImpl<N>;
	template<typename U, bool B> using IsT =  Is<U,LiteralType,B>;
	MatrixSlice<N> descriptor;
	std::unique_ptr<std::vector<T>> elements;
};
