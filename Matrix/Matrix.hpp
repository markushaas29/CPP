#include <vector>
#include <initializer_list>
#include <memory>
#include "Matrix_Ref.hpp" 
#include "Matrix_Slice.hpp" 
#include "Matrix_Initializer.hpp" 

template<size_t N, typename T = std::string>
class Matrix
{
public:
	static constexpr size_t Order = N;
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
//		descriptor.extents = MatrixImpl::derive_extents(init);
//		MatrixImpl::compute_strides(descriptor);
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
	MatrixSlice<N> descriptor;
	std::unique_ptr<std::vector<T>> elements;
};
