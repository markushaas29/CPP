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
	template<typename... Dims>
		MatrixSlice(Dims... dims);
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(Dims... dims) const
	{
		//static_assert
		std::size_t args[N] { size_t(dims)... };
		return start+std::inner_product(args,args+N,strides.begin(),size_t{0});
	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,N> extents;
	std::array<std::size_t,N> strides;
private:
};

template<>
class MatrixSlice<2>
{
public:
	MatrixSlice() = default;
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e);
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	template<typename... Dims>
		MatrixSlice(Dims... dims);
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t i, size_t j) const
	{
		return start+i*strides[0]+j;
	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,2> extents;
	std::array<std::size_t,2> strides;
private:
};

template<>
class MatrixSlice<1>
{
public:
	MatrixSlice() = default;
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e);
	MatrixSlice(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	template<typename... Dims>
		MatrixSlice(Dims... dims);
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t o) const
	{
		return o;
	}
	
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,1> extents;
	std::array<std::size_t,1> strides;
private:
};
