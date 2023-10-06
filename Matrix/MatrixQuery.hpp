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
class MatrixQuery 
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	MatrixQuery(){}
	
	decltype(auto) operator()(MatrixType* matrix, size_t i, const IMatrixCategory<ElementType>& cat) const 
	{
		if constexpr (MatrixType::Order==2)
        {
    	    typename MatrixType::IsT<Throwing>(Format("Index: ",i ," exceeds extents!"))(i<matrix->Cols());
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());

    	    for(int j = 0; j < matrix->Rows(); ++j)
    	    {
    	        auto row = matrix->row(j);
				if(cat(*row[i]))
    	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    	    }


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
	decltype(auto) operator()(MatrixType* matrix, const IMatrixCategory<ElementType>& cat) const 
	{
		if constexpr (MatrixType::Order==2)
        {
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());

    	    for(int j = 0; j < matrix->Rows(); ++j)
    	    {
    	        auto row = matrix->row(j);
    	    	for(int i = 0; i < row.size(); ++i)
					if(cat(*row[i]))
    	        		std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    	    }


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
private:
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixQuery& me) { return s<<me.matrix;  }

	template<size_t N>
	decltype(auto) copy(std::array<size_t,N> arr) const
	{
    	std::array<size_t,MatrixType::Order> res;
		std::copy(arr.begin(), arr.end(), res.begin());
		return res;
	}
};
