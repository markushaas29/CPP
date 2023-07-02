#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "Matrix_Ref.hpp"
#include "MatrixDescriptor.hpp"
#include "Matrix_Initializer.hpp"
#include "Matrix_Impl.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<std::size_t N, typename DT=MatrixDescriptor<N,int,int>>
class Matrix
{
public:
	using DescriptorType = DT;
	using Type = Matrix<N,DT>;
	using ParserType = typename DescriptorType::ParserType;
	using T = typename DescriptorType::InputType;
	using InputType = typename DT::InputType;
	using DataType = typename DT::DataType;
	using OutputTypes = typename DescriptorType::OutputTypes;
	static constexpr size_t Order = N;
	inline static constexpr const char TypeIdentifier[] = "Matrix";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	using ValueType = T;
	using Iterator = typename std::vector<T>::iterator;
	using ConstIterator = typename std::vector<T>::const_iterator;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;
	Matrix(const Matrix& m): descriptor(m.descriptor), elements{std::make_unique<std::vector<DataType>>(m.elements->cbegin(),m.elements->cend())}{ };
	Matrix& operator=(Matrix& m) { return Matrix(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}
	~Matrix() = default;

	explicit Matrix(DescriptorType d, const std::vector<DataType>& v): descriptor(d), elements{std::make_unique<std::vector<DataType>>(v.begin(),v.end())}{ };
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
	
	decltype(auto) AddRow(const std::vector<InputType>& v)
	{
		std::for_each(v.cbegin(), v.cend(), [&](auto i) { elements->push_back(std::make_shared<InputType>(i)); } );
		IsT<Throwing>(Format("Not jagged: Size: ",v.size()))(!MI::checkJagged(v.size(),descriptor));
		descriptor.AddRow();
	}

	decltype(auto) Row(size_t i)
    {  
    	assert(i<Rows());
		std::vector<DataType> result;
		for(auto r = i * Cols(); r < (i+1) * Cols(); r++)
			result.push_back(elements->at(r));
		return result;
    }
	
	decltype(auto) ElementsAt(size_t i)
    {
		auto r = Row(i);
		return parser.Parse(r);
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
	decltype(auto) Apply(F f)
	{
		auto el = std::vector<DataType>();
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& e) { el.push_back(std::make_shared<InputType>(f(e))); });
		return Type(descriptor,el); 
	}

  	decltype(auto) operator+(const auto& v)	{ return Apply([&](const auto& e){ return *e + v; });  	}
  	decltype(auto) operator+(const Type& m)	{ return apply(std::vector<DataType>(m.elements->cbegin(), m.elements->cend()), [&](const auto& e1, const auto& e2){ return *e1 + *e2; });  	}
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
};
