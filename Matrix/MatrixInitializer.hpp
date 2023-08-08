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
//
	MatrixInitializer(const auto& t) 
	{ 
		process<0>(t);
	}
	decltype(auto) Extents() { return extents; }
private:
	std::array<std::size_t,N> extents;
	friend std::ostream& operator<<(std::ostream& s, const MatrixInitializer& i) 
	{ 
		s<<"Extents: ";
		std::for_each(i.extents.cbegin(), i.extents.cend(), [&](size_t sz) { s<<sz<<" "; } );
		return s;  
	}
	template<size_t I>
	void process(const auto& v)
	{
		if constexpr (I<N)
		{
			extents[I] = v.size();
			process<I+1>(v.at(0));
		}

	}
};

template<typename T>
decltype(auto) Init(const std::vector<T>& v) { return MatrixInitializer<1>(v); }

template<typename T>
decltype(auto) Init(const std::vector<std::vector<T>>& v) { return MatrixInitializer<2>(v); }

template<typename T>
decltype(auto) Init(const std::vector<std::vector<std::vector<T>>>& v) { return MatrixInitializer<3>(v); }	
