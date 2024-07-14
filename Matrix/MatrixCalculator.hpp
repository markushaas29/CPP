#include <memory>
#include <tuple>
#include <vector>
#include "MatrixDescriptor.hpp"
#include "MatrixOperations.hpp"
#include "../Functional/Functional.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
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
		using ReturnType = Dot<typename LeftType::DataType,typename RightType::DataType>;
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
	template<template<typename> class OpT, typename It >
    static decltype(auto) calc(It begin, It end)
    {
		using Op = OpT<typename LeftType::ElementType>;

		auto v = std::vector<typename LeftType::ElementType>();
		std::for_each(begin,end, [&](auto i) { v.push_back(*i); });
		using DescriptorType = MatrixDescriptor<1, typename Op::ResultType>;
		using ResultType = Matrix<1, DescriptorType>;
        auto el = Op(v)();
        return ResultType(DescriptorType{el.size()},el); 
    }
    static decltype(auto) transform(const LeftType& m)
    {
		auto v = std::vector<typename LeftType::ElementType>(m.elements->size());
		for(size_t i = 0; i < m.Rows(); ++i)
			for(size_t j = 0; j < m.Cols(); ++j)
				v[(j * m.Cols())+i] = *(m.elements->at((i * m.Rows())+j));
		using ResultType = Matrix<LeftType::Order, DescriptorType>;
        return ResultType(DescriptorType({m.descriptor.Cols(), m.descriptor.Rows()}),v); 
	}
    static decltype(auto) transformDim(const LeftType& m, auto ND, auto... I)
    {
		std::cout<<"TRANS: "<<ND<<std::endl;
		auto v = std::vector<typename LeftType::ElementType>();
        for(auto i = 0; i < m.elements->size(); ++i) { v.push_back(*(m.elements->at(i))); };
		using ResultType = Matrix<LeftType::Order, DescriptorType>;
        return ResultType(DescriptorType({m.descriptor.Cols(), m.descriptor.Rows()}),v); 
    }
	template<typename F>
    static decltype(auto) apply(const LeftType& m, const F& f)
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
            el.push_back(std::make_shared<typename Op::ValueType>(Add(Constant(*(l.elements->at(i))),Constant(*(r.elements->at(i))))));
        return typename Op::MatrixType(d,el); 
    }
	static decltype(auto) sub(const LeftType& l,const RightType& r)
    {
        using Op = MatrixOperation<ElementSub,LeftType,RightType>;

        auto d = typename Op::DescriptorType(l.descriptor.Extents(), l.descriptor.Strides());
        auto el = std::vector<typename Op::DataType>();
        for(auto i = 0; i < l.elements->size(); ++i)
            el.push_back(std::make_shared<typename Op::ValueType>(Sub(Constant(*(l.elements->at(i))),Constant(*(r.elements->at(i))))));
        return typename Op::MatrixType(d,el); 
    }
	static decltype(auto) rowSum(const LeftType& l)
    {
		if constexpr (LeftType::Order==1)
			return l;
		if constexpr (LeftType::Order==2)
    	{
			auto d = MatrixDescriptor<1, Acc<typename LeftType::ElementType>>(l.Rows());
			auto el = std::vector<Acc<typename LeftType::ElementType>>();
			for(auto i = 0; i < l.Rows(); ++i)
			{
				std::vector<typename LeftType::ElementType> acc;
				for(auto j = 0; j < l.Cols(); ++j)
					acc.push_back(*l(i,j));
			    el.push_back(Acc(acc));
			}

			return Matrix<1, decltype(d)>(d,el); 
    	}
    }
	static decltype(auto) colSum(const auto& l, int i)
    {
		std::vector<typename LeftType::ElementType> acc;
		for(auto j = 0; j < l.Rows(); ++j)
			acc.push_back(*l(j,i));
		return Acc(acc);	
    }
	static decltype(auto) colSum(const Matrix<Order, MatrixDescriptor<Order, std::string>>& l, int i)
    {
		std::vector<double> acc;
		for(auto j = 0; j < l.Rows(); ++j)
			acc.push_back(To<double>(*l(j,i)));
		return Acc(acc);	
    }
	
	static decltype(auto) colSum(const auto& l)
    {
		if constexpr (LeftType::Order==1)
		{
			std::vector<typename LeftType::ElementType> acc;
			for(auto j = 0; j < l.Rows(); ++j)
				acc.push_back(*l(j));
			return Acc(acc);	
		}
		if constexpr (LeftType::Order==2)
    	{
			auto d = MatrixDescriptor<1, Acc<typename LeftType::ElementType>>(l.Cols());
			std::vector<Acc<typename LeftType::ElementType>> av;
			for(auto i = 0; i < l.Cols(); ++i)
				av.push_back(l.ColSum(i));
			return Matrix<1, decltype(d)>(d,av); 
    	}
    }

	static decltype(auto) colSum(const Matrix<Order, MatrixDescriptor<Order, std::string>>& l)
    {
		if constexpr (LeftType::Order==1)
		{
			std::vector<double> acc;
			for(auto j = 0; j < l.Rows(); ++j)
				acc.push_back(To<double>(*l(j)));
			return Acc(acc);	
		}
		if constexpr (LeftType::Order==2)
    	{
			auto d = MatrixDescriptor<1, Acc<double>>(l.Cols());
			std::vector<Acc<double>> av;
			for(auto i = 0; i < l.Cols(); ++i)
				av.push_back(l.ColSum(i));
			return Matrix<1, decltype(d)>(d,av); 
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
				v.push_back(std::make_shared<typename Base::DotProductType::ReturnType>(Dot<typename Base::LeftType::DataType, typename Base::RightType::DataType>(l.row(i),r.col(j))));

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
			v[i] = std::make_shared<typename Base::DotProductType::ReturnType>(Dot(l.row(i),r.col(0)));

		return typename Base::DotProductType::ResultType(md,v);
	}
	
	static decltype(auto) divide(const Base::LeftType& l, const Base::RightType& r) 
	{
		typename Base::template IsT<Throwing>("M2M1")(r.Cols()>=1);
		using ReturnType = Div<Constant<typename Base::LeftType::ElementType>, Constant<typename Base::RightType::ElementType>>;
	 	using DescriptorType = MatrixDescriptor<2, ReturnType>;
		DescriptorType md({l.Rows(), r.Rows()});
 	    using ResultType = Matrix<2, DescriptorType>;

		std::vector<std::shared_ptr<ReturnType>> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != r.Rows(); ++j)
				v.push_back(std::make_shared<ReturnType>(ReturnType( Constant(*l(i,j)),Constant(*r(j) ))));

		return ResultType(md,v);
	}
};

template<typename D1, typename D2>
class MatrixCalculator<Matrix<1,D1>, Matrix<2,D2>> : public MatrixCalculatorBase<MatrixCalculator,1,Matrix<1,D1>, Matrix<2,D2>> 
{
	using Base = MatrixCalculatorBase<MatrixCalculator,1,Matrix<1,D1>, Matrix<2,D2>>;
private:
	friend class Matrix<1,D1>;
	static decltype(auto) multiply(const Base::LeftType& l, const Base::RightType& r) 
	{
		typename Base::template IsT<Throwing>("M1M2")(r.Cols()>=1);
		typename Base::DotProductType::DescriptorType md({l.Rows()});

		std::vector<std::shared_ptr<typename Base::DotProductType::ReturnType>> v(l.Rows());
		for(int i = 0; i != l.Rows(); ++i)
			v[i] = std::make_shared<typename Base::DotProductType::ReturnType>(Dot(l.col(0),r.col(i)));

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
		using ReturnType = Mul<Constant<typename Base::LeftType::ElementType>,Constant<typename Base::RightType::ElementType>>;
	 	using DescriptorType = MatrixDescriptor<Base::Order, ReturnType>;
 	    using ResultType = Matrix<Base::Order, DescriptorType>;
		
		DescriptorType md({l.Rows(), r.Rows()});
		std::vector<std::shared_ptr<ReturnType>> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != r.Rows(); ++j)
				v.push_back(std::make_shared<ReturnType>(Mul( Constant(*l(i)),Constant(*r(j) ))));

		return ResultType(md,v);
	}
	
	static decltype(auto) divide(const Base::LeftType& l, const Base::RightType& r) 
	{
		typename Base::template IsT<Throwing>("M1M1")(r.Rows()==l.Rows());
		using ReturnType = Div<Constant<typename Base::LeftType::ElementType>, Constant<typename Base::RightType::ElementType>>;
	 	using DescriptorType = MatrixDescriptor<1, ReturnType>;
		DescriptorType md({l.Rows()});
 	    using ResultType = Matrix<1, DescriptorType>;

		std::vector<std::shared_ptr<ReturnType>> v;
		for(int i = 0; i != l.Rows(); ++i)
				v.push_back(std::make_shared<ReturnType>(ReturnType( Constant(*l(i)),Constant(*r(i) ))));
		return ResultType(md,v);
	}
};
