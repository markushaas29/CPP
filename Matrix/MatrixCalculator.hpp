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
protected:
	template<typename V>
	static V dotProduct (const std::vector<V>& v1, const std::vector<V>& v2)
	{
		double res;
		for(uint i = 0; i < v1.size(); ++i)
			res += (*v1[i] * (*v2[i]));
		return std::make_shared<int>(res);
	}
private:
};
template<typename M1, typename M2>
class MatrixCalculator 
{
public:

private:
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<2,D1>, Matrix<1,D2>> : public MatrixCalculatorBase<MatrixCalculator,1,Matrix<2,D1>, Matrix<1,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,1,Matrix<2,D1>, Matrix<1,D2>>;
private:
	friend class Matrix<2,D1>;
	static decltype(auto) multiply(Base::LeftType l, Base::RightType r) 
	{
		assert(l.Cols()==r.Rows());
		typename Base::DescriptorType md({l.Rows()});

		std::vector<typename Base::LeftType::DataType> v(l.Rows());
		for(int i = 0; i != l.Rows(); ++i)
			v[i] = Base::dotProduct(l.Row(i),r.Col(0));

		return typename Base::ResultType(md,v);
	}
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

		auto in = Base::dotProduct(l.Row(0),r.Row(0));
		std::cout<<"DOT "<<*in<<"\n";

		std::vector<typename Base::LeftType::DataType> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != r.Rows(); ++j)
				v.push_back(std::make_shared<typename Base::LeftType::InputType>((*l(i)) * (*r(j)) ));

		return typename Base::ResultType(md,v);
	}
};
