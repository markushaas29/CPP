#include <vector>
#include "MatrixDescriptor.hpp" 

#pragma once

template<size_t N, typename ET>
class MatrixInitializer 
{
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixInitializer";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = N;
//	using Type = Matrix<N,DT>;
//	using DescriptorType = DT;
	using ElementType = ET;
//	using DataType = typename DT::DataType;
//	using ValueType = ElementType;
//
	MatrixInitializer(const auto& t): elements{process<0>(t,t)}, descriptor{extents}
	{ 
	}
	decltype(auto) Extents() { return extents; }
private:
	std::array<std::size_t,N> extents;
	std::vector<ElementType> elements;
	MatrixDescriptor<N,ElementType> descriptor;
	friend std::ostream& operator<<(std::ostream& s, const MatrixInitializer& i) {	return s<<"Size: "<<i.elements.size()<<"\n"<<i.descriptor;	}
	template<size_t I>
	decltype(auto) process(const auto& v, const auto result)
	{
		if constexpr (I==N)
			return result;
		if constexpr (I<N)
		{
			auto r = flatten(result);
			extents[I] = v.size();
			return process<I+1>(v.at(0),r);
		}

	}

	template <typename T>
	std::vector<T> flatten(const std::vector<std::vector<T>>& v) {
	    std::size_t total_size = 0;
	    for (const auto& sub : v)
	        total_size += sub.size();
	    std::vector<T> result;
	    result.reserve(total_size);
	    for (const auto& sub : v)
	        result.insert(result.end(), sub.begin(), sub.end());
	    return result;
	}
	
	template <typename T>
	std::vector<T> flatten(const std::vector<T>& v) { return v;	}
};

template<typename T>
decltype(auto) Init(const std::vector<T>& v) { return MatrixInitializer<1,T>(v); }

template<typename T>
decltype(auto) Init(const std::vector<std::vector<T>>& v) { return MatrixInitializer<2,T>(v); }

template<typename T>
decltype(auto) Init(const std::vector<std::vector<std::vector<T>>>& v) { return MatrixInitializer<3,T>(v); }	

