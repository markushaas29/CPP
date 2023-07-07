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
class MatrixCalculator<Matrix<1,D1>, Matrix<1,D2>> 
{
	using LT = Matrix<1,D1>;
	using RT = Matrix<1,D2>;
	using DescT = MatrixDescriptor<2, typename LT::InputType, typename LT::OutputTypes>;
	using ResultType = Matrix<2, DescT>;
private:
	friend class Matrix<1,D1>;
	static decltype(auto) multiply(LT l, RT r) 
	{
		DescT md({l.Rows(), r.Rows()});

		std::vector<typename LT::DataType> v;
		for(int i = 0; i != l.Rows(); ++i)
			for(int j = 0; j != l.Rows(); ++j)
				v.push_back(std::make_shared<typename LT::InputType>((*l(i)) * (*r(j)) ));

		return ResultType(md,v);
	}
};
