#include <memory>
#include <tuple>
#include <vector>
#include "MatrixDescriptor.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#pragma once

template<std::size_t N, typename DT> class Matrix;

template<template<typename, typename> class T, uint N, typename M1, typename M2>
class MatrixCalculatorBase 
{
public:
	static constexpr uint Order = N;
	using LeftType = M1;
	using RightType = M2;
	using DescriptorType = MatrixDescriptor<Order, typename LeftType::InputType, typename LeftType::OutputTypes>;
	using ResultType = Matrix<Order, DescriptorType>;

private:
};
template<typename M1, typename M2>
class MatrixCalculator 
{
public:

private:
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<2,D1>, Matrix<2,D2>> 
{
public:
	static decltype(auto) multiply() { std::cout<<"2/2"<<std::endl; }
private:
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<1,D1>, Matrix<1,D2>>: public MatrixCalculatorBase<MatrixCalculator,2,Matrix<1,D1>, Matrix<1,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,2,Matrix<1,D1>, Matrix<1,D2>>;
private:
	friend class Matrix<1,D1>;
	static decltype(auto) multiply(Base::LeftType l, Base::RightType r) 
	{
		typename Base::DescriptorType md({l.Rows(), r.Rows()});

		std::vector<typename Base::LeftType::DataType> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != l.Rows(); ++j)
				v.push_back(std::make_shared<typename Base::LeftType::InputType>((*l(i)) * (*r(j)) ));

		return typename Base::ResultType(md,v);
	}
};
