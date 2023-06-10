#include <array>
#include <numeric>
#include <initializer_list>

#pragma once

constexpr bool All() { return true; }

template<typename... Args>
constexpr bool All(bool b, Args... a) { return b && All(a...); }

template<size_t N> class MatrixImpl;

template<size_t N, typename T>
class MatrixDescriptorBase
{
	friend class MatrixImpl<N>;
public:
	MatrixDescriptorBase() = default;
	MatrixDescriptorBase(std::size_t o, std::initializer_list<size_t> e);
	MatrixDescriptorBase(std::size_t o, std::initializer_list<size_t> e, std::initializer_list<size_t> s);
	MatrixDescriptorBase(auto... dims)
	{
       std::initializer_list<int> il({dims...} );
 	   std::copy (il.begin(), il.end(), extents.begin());
	};
	MatrixDescriptorBase(std::array<std::size_t,N> e, std::array<std::size_t,N> s): extents{e}, strides{s}, size{e[0] * s[0]} { };
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(Dims... dims) const
	{
		std::size_t args[N] { size_t(dims)... };
		return start+std::inner_product(args,args+N,strides.begin(),size_t{0});
	}

	const std::array<size_t,N>& Strides() const { return strides; }
	const std::array<size_t,N>& Extents() const { return extents; }
	decltype(auto) Size() const { return size; }
	decltype(auto) Start() const { return start; }
	decltype(auto) Stride(auto i) const { return strides[i]; }
	decltype(auto) Extent(auto i) const { return extents[i]; }
	decltype(auto) SetExtents(auto e) { extents = e; }
	decltype(auto) AddRow() { 
		std::array<std::size_t,N> e;
		e = extents;
		e[0] = extents[0] + 1;
		extents = e; }
private:
	std::size_t size;
	std::size_t start;
	std::array<std::size_t,N> extents;
	std::array<std::size_t,N> strides;
	friend std::ostream& operator<<(std::ostream& s, const MatrixDescriptorBase& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};
template<size_t N, typename T=std::string>
class MatrixDescriptor: public MatrixDescriptorBase<N,T>
{
	using Base = MatrixDescriptorBase<N,T>;
public:
	MatrixDescriptor() = default;
	MatrixDescriptor(auto... dims): Base(dims...) {	};
	MatrixDescriptor(std::array<std::size_t,N> e, std::array<std::size_t,N> s): Base{e,s} {};
};

template<typename T>
class MatrixDescriptor<2,T>: public MatrixDescriptorBase<2,T>
{
	using Base = MatrixDescriptorBase<2,T>;
public:
	MatrixDescriptor() = default;
	MatrixDescriptor(auto... dims): Base(dims...) {	};
	MatrixDescriptor(std::array<std::size_t,2> e, std::array<std::size_t,2> s): Base{e,s} {};

	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t i, size_t j) const	{	return Base::Start()+i*Base::Strides(0)+j;	}
};

template<typename T>
class MatrixDescriptor<1,T>: public MatrixDescriptorBase<1,T>
{
	using Base = MatrixDescriptorBase<1,T>;
public:
	MatrixDescriptor() = default;
	MatrixDescriptor(std::array<std::size_t,1> e, std::array<std::size_t,1> s): Base{e,s} {};
	
	template<typename... Dims, typename std::enable_if<All(std::is_convertible<Dims...,std::size_t>::value),void>::type>
	std::size_t operator()(size_t o) const	{	return o;	}
};

