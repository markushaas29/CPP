#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixCategory.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"

#pragma once


template<typename T>
class IMatrixQuery
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	
	template<typename CT>
	MatrixType operator()( MatrixType* matrix, const IMatrixCategory<CT>& cat) const
	{
		if constexpr (MatrixType::Order==2)
        {
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());

    	    for(int j = 0; j < matrix->Rows(); ++j)
    	        exec(result, matrix->row(j), cat);


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
private:
	virtual void exec(std::vector<typename MatrixType::DataType>& result, const std::vector<typename MatrixType::DataType>& row, const IMatrixCategory<ElementType>& cat) const = 0;
	template<size_t N>
	decltype(auto) copy(std::array<size_t,N> arr) const
	{
    	std::array<size_t,MatrixType::Order> res;
		std::copy(arr.begin(), arr.end(), res.begin());
		return res;
	}
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const IMatrixQuery& me) { return s;  }
};

template<typename T>
class MatrixQuery:public IMatrixQuery<T> 
{
	using Base = IMatrixQuery<T>;
public:
	MatrixQuery(){}
private:
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, const IMatrixCategory<typename Base::ElementType>& cat) const
	{
    	for(int i = 0; i < row.size(); ++i)
			if(cat(*row[i]))
    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
};

template<typename T>
class MatrixColQuery: public IMatrixQuery<T> 
{
	using Base = IMatrixQuery<T>;
public:
	MatrixColQuery(size_t c): col{c} {}
private:
	size_t col;
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, const IMatrixCategory<typename Base::ElementType>& cat) const
	{
		if(cat(*row[col]))
        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
};
