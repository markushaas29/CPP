#include <array>
#include <numeric>
#include <initializer_list>

#pragma once

constexpr bool All() { return true; }

template<typename... Args>
constexpr bool All(bool b, Args... a) { return b && All(a...); }

template<size_t N>
class MatrixSlice
{
public:
	MatrixSlice() = default;
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e);
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	MatrixSlice(auto... dims)
	{
          //IsT<Throwing>("Dim != N")(Dim==N);
       std::initializer_list<int> il({dims...} );
 	   std::copy (il.begin(), il.end(), extents.begin());
	};
	MatrixSlice(std::array<std::size_t,N> e, std::array<std::size_t,N> s): extents{e}, strides{s}, size{e[0] * s[0]} {};
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(Dims... dims) const
	{
		std::size_t args[N] { size_t(dims)... };
		return start+std::inner_product(args,args+N,strides.begin(),size_t{0});
	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,N> extents;
	std::array<std::size_t,N> strides;
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixSlice& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};

template<>
class MatrixSlice<2>
{
public:
	MatrixSlice() = default;
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e);
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	template<typename... Dims>	MatrixSlice(Dims... dims)
	{
          //IsT<Throwing>("Dim != N")(Dim==N);
       std::initializer_list<int> il({dims...} );
 	   std::copy (il.begin(), il.end(), extents.begin());
	};
	MatrixSlice(std::array<std::size_t,2> e, std::array<std::size_t,2> s): extents{e}, strides{s}, size{e[0] * s[0]} {};

	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t i, size_t j) const	{	return start+i*strides[0]+j;	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,2> extents;
	std::array<std::size_t,2> strides;
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixSlice& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};

template<>
class MatrixSlice<1>
{
public:
	MatrixSlice() = default;
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e);
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	MatrixSlice(std::array<std::size_t,1> e, std::array<std::size_t,1> s): extents{e}, strides{s}, size{e[0] * s[0]} {};
	
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t o) const	{	return o;	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,1> extents;
	std::array<std::size_t,1> strides;
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixSlice& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};
