#include <vector>
#include <initializer_list>
#include <memory>
#include "MatrixDescriptor.hpp"
#include "MatrixElement.hpp"
#include "MatrixCalculator.hpp"
#include "MatrixInitializer.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../Calculator/Operations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename T = std::string, typename DT=MatrixDescriptor<3,T>>
class M3
{
public:
	inline static constexpr const char TypeIdentifier[] = "M3";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = 3;
	using Type = M3<T,DT>;
	using DescriptorType = DT;
	using ElementType = typename DT::ElementType;
	using DataType = typename DT::DataType;
	using ValueType = ElementType;
	using VecType = std::vector<std::vector<std::vector<ElementType>>>;

	M3() = delete;
	M3(M3&&) = default;
	M3& operator=(M3&&) = default;
	~M3() = default;

//	M3(const M3& m): descriptor(m.descriptor), elements{std::make_unique<std::vector<DataType>>(m.elements->cbegin(),m.elements->cend())} { check();};
	explicit M3(const VecType& v):  elements{std::make_unique<VecType>(v.begin(),v.end())}{  };

	M3& operator=(M3& m) { return M3(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}

	//decltype(auto) Rows() const { return descriptor.Rows(); }
	//decltype(auto) Cols() const { return descriptor.Cols(); }
//	size_t Extent(size_t n) const { return descriptor.Extents()[n]; }
//	size_t Size() const { return descriptor.Size(); }
//	const DescriptorType& Descriptor() const { return descriptor; }

	decltype(auto) operator[] (size_t i) const { return Init(elements->at(i))(); }
//	decltype(auto) AddRow(const std::vector<ElementType>& v) { access->addRow(v,this); }
//	decltype(auto) Col(size_t i) const { return access->colAt(i, this); }
//	decltype(auto) Cols(auto... i) const { return access->cols(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
//	decltype(auto) Rows(auto... i) const { return access->rows(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
//	template<typename T>
//	decltype(auto) To() const { return access->template to<T>(this); }

//	template<typename F>
//	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
private:
//	template<typename U> using IsT =  Is<U,LiteralType>;
//	template<typename T> using MC = M3Calculator<Type, T>;
//	using MI = M3Impl<N,DescriptorType>;
//	
//	decltype(auto) operator() (auto... I) const
//	{
//		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
//		auto i = MI::computePosition(descriptor.Extents(),descriptor.Strides(), I...);
//		return elements->at(i); 
//	}
//	
//	decltype(auto) row(size_t i) const { return access->row(i, this); }
//	decltype(auto) col(size_t i) const { return access->col(i, this); }
//
//	friend std::ostream& operator<<(std::ostream& s, const M3& m) { return (*m.io)(s,&m); }
//	template<typename,typename> friend class M3Calculator;
//	template<template<typename, typename> class T, uint, typename, typename> friend class M3CalculatorBase;
//	friend class M3Access<Type>;
//	friend class M3IO<Type>;
//	friend class M3Filter<Type>;
//
//	DescriptorType descriptor;
//	std::unique_ptr<M3Access<Type>> access = std::make_unique<M3Access<Type>>();
//	std::unique_ptr<M3IO<Type>> io = std::make_unique<M3IO<Type>>();
	std::unique_ptr<VecType> elements = std::make_unique<std::vector<DataType>>();
};
