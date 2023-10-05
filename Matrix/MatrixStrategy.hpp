#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once

template<std::size_t, typename> class Matrix;

template<typename T>
class IMatrixStrategy
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	virtual T operator()(const T& m) = 0;
};

template<typename T>
class UnaryMatrixStrategy : public IMatrixStrategy<T>
{
	using Base = IMatrixStrategy<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	UnaryMatrixStrategy(typename Base::ElementType e): matrix(e) {}
	
	virtual Base::MatrixType operator()(const Base::MatrixType& m) { std::cout<<"Un"<<std::endl;return typename Base::MatrixType(); };
	//const MatrixType& operator()() const { return matrix; } 
	//decltype(auto) operator()(size_t i, std::function<bool(const typename MatrixType::ElementType& i)> pred = [](const typename MatrixType::ElementType& e) { return e==0; }) const 
	//{
	//	if constexpr (MatrixType::Order==2)
    //    {
    //	    typename MatrixType::IsT<Throwing>(Format("Index: ",i ," exceeds extents!"))(i<matrix.Cols());
    //	    std::vector<typename MatrixType::DataType> result;
    //	    std::array<size_t,MatrixType::Order> e = copy(matrix.descriptor.Extents());

    //	    for(int j = 0; j < matrix.Rows(); ++j)
    //	    {
    //	        auto row = matrix.row(j);
	//			if(pred(*row[i]))
    //	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    //	    }


    //	    e[0] = result.size() / matrix.Cols();
    //	    
	//		return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix.descriptor.Strides())}, result);
    //    }
	//}
	//decltype(auto) operator()(std::function<bool(const std::vector<typename MatrixType::DataType>& i)> pred) const 
	//{
	//	if constexpr (MatrixType::Order==2)
    //    {
    //	    std::vector<typename MatrixType::DataType> result;
    //	    std::array<size_t,MatrixType::Order> e = copy(matrix.descriptor.Extents());
    //	    
	//		for(int i = 0; i < matrix.Rows(); ++i)
    //	    {
    //	        auto row = matrix.row(i);
	//			if(pred(row))
    //	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    //	    }
    //	    
    //	    e[0] = result.size() / matrix.Cols();
    //	    
	//		return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix.descriptor.Strides())}, result);
    //    }
	//}
private:
	Base::ElementType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	//friend std::ostream& operator<<(std::ostream& s, const MatrixStrategy& me) { return s<<me.matrix;  }
};

template<typename T>
class BinaryMatrixStrategy : public IMatrixStrategy<T>
{
	using Base = IMatrixStrategy<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixStrategy";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	BinaryMatrixStrategy(std::vector<typename Base::ElementType> e): matrix(e) {}
	
	virtual Base::MatrixType operator()(const Base::MatrixType& m) { std::cout<<"Bin"<<std::endl; return typename Base::MatrixType(); };
private:
	std::vector<typename Base::ElementType> matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
};
