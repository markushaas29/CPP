#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "Matrix_Ref.hpp"
#include "MatrixDescriptor.hpp"
#include "Matrix_Initializer.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixCell.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<std::size_t N, typename DT=MatrixDescriptor<N,int,int>>
class Matrix
{
public:
	using DescriptorType = DT;
	using T = typename DescriptorType::DataType;
	using OutputTypes = typename DescriptorType::OutputTypes;
	static constexpr size_t Order = N;
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using InputType = typename DT::InputType;
	using ValueType = T;
	using Iterator = typename std::vector<T>::iterator;
	using ConstIterator = typename std::vector<T>::const_iterator;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	Matrix(const Matrix&) = default;
	Matrix& operator=(Matrix&) = default;
	~Matrix() = default;

	explicit Matrix(DescriptorType d, const std::vector<T>& v): descriptor(d), elements{std::make_unique<std::vector<T>>(v.begin(),v.end())}{ };
	Matrix(MatrixInitializer<T,N> init)
	{
		descriptor.SetExtents(MI::derive_extents(init));
		MI::compute_strides(descriptor);
		elements->reserve(descriptor.Size());
		MI::insert_flat(init,elements);
	};
	Matrix& operator=(MatrixInitializer<T,N>) {};

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
	decltype(auto) operator[](size_t i) 
	{
		using MDT = MatrixDescriptor<N-1,typename DescriptorType::InputType>;
		std::array<size_t,N-1> e;
		std::array<size_t,N-1> s;
		std::copy(descriptor.Extents().begin()+1, descriptor.Extents().end(), e.begin());
		std::copy(descriptor.Strides().begin()+1, descriptor.Strides().end(), s.begin());
		auto row = Row(i);
		if constexpr ((N-1)==0)
		{
			auto ret = row[0];
			return ret;
		}
		else
			return Matrix<N-1,MDT>(MDT{e,s}, row);
	}

	Matrix<N-1,const T> operator[](size_t i) const;// { return Row(i); }
	
	decltype(auto) AddRow(const std::vector<T>& v)
	{
		elements->insert(elements->end(), v.begin(), v.end());
		descriptor.AddRow();
	}

	decltype(auto) Row(size_t i)
    {  
    	assert(i<Rows());
		std::vector<T> result;
		for(auto r = i * Cols(); r < (i+1) * Cols(); r++)
			result.push_back(elements->at(r));
		return result;
    }

	decltype(auto) Col(size_t i)
    {  
    	assert(i<Cols());
		std::vector<T> result;
		for(auto r = 0; r < Rows(); r++)
			result.push_back(elements->at(i + (r * Cols())));
		return result;
    }
private:
	friend std::ostream& operator<<(std::ostream& s, const Matrix& m) 
	{
		s<<"Extents: \n";
		std::for_each(m.descriptor.Extents().cbegin(), m.descriptor.Extents().cend(), [&s](const auto& i) { s<<i<<"\t"; });
		s<<"\nStride: \n";
		std::for_each(m.descriptor.Strides().cbegin(), m.descriptor.Strides().cend(), [&s](const auto& i) { s<<i<<"\t"; });
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

	using MI = MatrixImpl<N,DescriptorType>;
	template<typename U, bool B> using IsT =  Is<U,LiteralType,B>;
	DescriptorType descriptor;
	std::unique_ptr<std::vector<T>> elements = std::make_unique<std::vector<T>>();
};
