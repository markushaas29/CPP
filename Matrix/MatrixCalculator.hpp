#include <memory>
#include <tuple>
#include <vector>
#include "MatrixDescriptor.hpp"
#include "MatrixOperations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Calculator/Operations.hpp"
#pragma once

template<std::size_t N, typename DT> class Matrix;

template<template<typename, typename> class T, uint N, typename M1, typename M2>
class MatrixCalculatorBase 
{
public:
	static constexpr uint Order = N;
	using LeftType = M1;
	using RightType = M2;
	using DescriptorType = MatrixDescriptor<Order, typename LeftType::ElementType>;
	using ResultType = Matrix<Order, DescriptorType>;
	inline static constexpr const char TypeIdentifier[] = "MatrixCaluclator";
	inline static constexpr Literal LiteralType{TypeIdentifier};
protected:
	template<typename U> using IsT =  Is<U,LiteralType>;
	template<typename V>
	static decltype(auto) dotProduct (const std::vector<V>& v1, const std::vector<V>& v2)
	{
		typename V::element_type res = 0;
		for(uint i = 0; i < v1.size(); ++i)
			res += (*v1[i] * (*v2[i]));
		return std::make_shared<typename V::element_type>(res);
	}
	struct DotProductType
	{
		using ReturnType = decltype(DotProduct::Calculate(std::declval<std::vector<typename LeftType::DataType>>(),std::declval<std::vector<typename RightType::DataType>>()));
	 	using DescriptorType = MatrixDescriptor<Order, ReturnType>;
 	    using ResultType = Matrix<Order, DescriptorType>;
	};
private:
	template<typename F, typename It >
    static decltype(auto) apply(F f, It begin, It end, DescriptorType d)
    {
        auto el = std::vector<typename LeftType::DataType>();
        std::for_each(begin, end, [&](const auto& e) { el.push_back(std::make_shared<typename LeftType::ElementType>(f(e))); });
        return LeftType(d,el); 
    }
	template<typename F>
    static decltype(auto) apply(const LeftType& m, F f)
    {
        using Op = ValueOperation<F, LeftType, typename LeftType::ElementType>;

        auto d = typename Op::DescriptorType(m.descriptor.Extents(), m.descriptor.Strides());
        auto el = std::vector<typename Op::DataType>();
        std::for_each(m.elements->cbegin(), m.elements->cend(), [&](const auto& e) { el.push_back(std::make_shared<typename Op::ValueType>(f(*e))); });
        return typename Op::MatrixType(d,el); 
    }
	template<size_t, typename> friend class Matrix;
	static decltype(auto) add(const LeftType& l,const RightType& r)
    {
        using Op = MatrixOperation<ElementAdd,LeftType,RightType>;

        auto d = typename Op::DescriptorType(l.descriptor.Extents(), l.descriptor.Strides());
        auto el = std::vector<typename Op::DataType>();
        for(auto i = 0; i < l.elements->size(); ++i)
            el.push_back(std::make_shared<typename Op::ValueType>(Addition::Calculate(*(l.elements->at(i)),*(r.elements->at(i)))));
        return typename Op::MatrixType(d,el); 
    }
	static decltype(auto) sub(const LeftType& l,const RightType& r)
    {
        using Op = MatrixOperation<ElementSub,LeftType,RightType>;

        auto d = typename Op::DescriptorType(l.descriptor.Extents(), l.descriptor.Strides());
        auto el = std::vector<typename Op::DataType>();
        for(auto i = 0; i < l.elements->size(); ++i)
            el.push_back(std::make_shared<typename Op::ValueType>(Subtraction::Calculate(*(l.elements->at(i)),*(r.elements->at(i)))));
        return typename Op::MatrixType(d,el); 
    }
	static decltype(auto) rowSum(const LeftType& l)
    {
		if constexpr (LeftType::Order==1)
		{
			double res = 0.0;
			for(auto i = 0; i < l.Rows(); ++i)
				res += (double)l[i];
			return MatrixElement<typename LeftType::ElementType>(res);
		}
		if constexpr (LeftType::Order==2)
    	{
			auto d = MatrixDescriptor<1, typename LeftType::ElementType>(l.Rows());
			auto el = std::vector<typename LeftType::DataType>();
			for(auto i = 0; i < l.Rows(); ++i)
			{
				double cs = 0.0;
				for(auto j = 0; j < l.Cols(); ++j)
					cs += (double)(*l(i,j));
			    el.push_back(std::make_shared<typename LeftType::ElementType>(cs));
			}

			return Matrix<1, decltype(d)>(d,el); 
    	}
    }
	static decltype(auto) colSum(const LeftType& l, int i)
    {
		auto el = std::vector<typename LeftType::DataType>();
		if constexpr (LeftType::Order==1)
			el = l.col(i);
		if constexpr (LeftType::Order==2)
			el = l.col(i);
		
		double cs = 0.0;
		for(auto j = 0; j < l.Rows(); ++j)
		{
			if constexpr (std::is_same_v<std::string, typename LeftType::ElementType>)
			{
				std::string s(*el.at(j));
                std::replace(s.begin(), s.end(), ',', '.');
                std::istringstream iss(static_cast<std::string>(s));
                double val;
                iss>>val;

				cs += val;
			}
			else
				cs += (double)(*el(j));
		}
		return cs;	
    }
	static decltype(auto) colSum(const LeftType& l)
    {
		if constexpr (LeftType::Order==1)
			return l;
		if constexpr (LeftType::Order==2)
    	{
			auto d = MatrixDescriptor<1, typename LeftType::ElementType>(l.Cols());
			auto el = std::vector<typename LeftType::DataType>();
			for(auto i = 0; i < l.Cols(); ++i)
			{
				double cs = 0.0;
				for(auto j = 0; j < l.Rows(); ++j)
					cs += (double)(*l(j,i));
			    el.push_back(std::make_shared<typename LeftType::ElementType>(cs));
			}

			return Matrix<1, decltype(d)>(d,el); 
    	}
    }
};

template<typename M1, typename M2>class MatrixCalculator {};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<2,D1>, Matrix<2,D2>> : public MatrixCalculatorBase<MatrixCalculator,2,Matrix<2,D1>, Matrix<2,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,2,Matrix<2,D1>, Matrix<2,D2>>;
private:
	friend class Matrix<2,D1>;
	static decltype(auto) multiply(const Base::LeftType& l, const Base::RightType& r) 
	{
		typename Base::template IsT<Throwing>("M2M2")(l.Cols()==r.Rows());
		typename Base::DotProductType::DescriptorType md({l.Rows(), r.Cols()});

		std::vector<std::shared_ptr<typename Base::DotProductType::ReturnType>> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != r.Cols(); ++j)
				v.push_back(std::make_shared<typename Base::DotProductType::ReturnType>(DotProduct::Calculate(l.row(i),r.col(j))));

		return typename Base::DotProductType::ResultType(md,v);
	}
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<2,D1>, Matrix<1,D2>> : public MatrixCalculatorBase<MatrixCalculator,1,Matrix<2,D1>, Matrix<1,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,1,Matrix<2,D1>, Matrix<1,D2>>;
private:
	friend class Matrix<2,D1>;
	static decltype(auto) multiply(const Base::LeftType& l, const Base::RightType& r) 
	{
		typename Base::template IsT<Throwing>("M2M1")(r.Cols()>=1);
		typename Base::DotProductType::DescriptorType md({l.Rows()});

		std::vector<std::shared_ptr<typename Base::DotProductType::ReturnType>> v(l.Rows());
		for(int i = 0; i != l.Rows(); ++i)
			v[i] = std::make_shared<typename Base::DotProductType::ReturnType>(DotProduct::Calculate(l.row(i),r.col(0)));

		return typename Base::DotProductType::ResultType(md,v);
	}
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<1,D1>, Matrix<1,D2>>: public MatrixCalculatorBase<MatrixCalculator,2,Matrix<1,D1>, Matrix<1,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,2,Matrix<1,D1>, Matrix<1,D2>>;
private:
	friend class Matrix<1,D1>;
	static decltype(auto) multiply(const Base::LeftType& l, const Base::RightType& r) 
	{
		//IsT<Throwing>("M2M1")(l.R()==r.Rows());
		using ReturnType = decltype(Multiplication::Calculate(std::declval<typename Base::LeftType::ElementType>(),std::declval<typename Base::RightType::ElementType>()));
	 	using DescriptorType = MatrixDescriptor<Base::Order, ReturnType>;
 	    using ResultType = Matrix<Base::Order, DescriptorType>;
		
		DescriptorType md({l.Rows(), r.Rows()});
		std::vector<std::shared_ptr<ReturnType>> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != r.Rows(); ++j)
				v.push_back(std::make_shared<ReturnType>(Multiplication::Calculate( *l(i),*r(j) )));

		return ResultType(md,v);
	}
};
