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

template<typename T>
class MatrixFilter 
{
public:
	using MatrixType = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixFilter_UniqueElement";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	decltype(auto) operator()() const { return elements.erase(std::unique(elements.begin(), elements.end()),elements.end()); } 
	{
		if constexpr (MatrixType::Order==2)
        {
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e = copy(matrix.descriptor.Extents());
    	    
			for(int i = 0; i < matrix.Rows(); ++i)
    	    {
    	        auto row = matrix.row(i);
				if(pred(row))
    	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    	    }
    	    
    	    e[0] = result.size() / matrix.Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix.descriptor.Strides())}, result);
        }
	}
private:
	std::vector<typename MatrixType::ElementType> elements;
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFilter& me) { return s;  }
};
