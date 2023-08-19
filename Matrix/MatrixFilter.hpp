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
	inline static constexpr const char TypeIdentifier[] = "MatrixFilter";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	using InputIterator = std::vector<std::string>::const_iterator;

	MatrixFilter(MatrixType m): matrix(m) {}
	
	decltype(auto) operator()(size_t i, std::function<bool(const typename MatrixType::ElementType& i)> pred = [](const typename MatrixType::ElementType& e) { return e==0; }) const 
	{
		if constexpr (MatrixType::Order==2)
        {
    	    typename MatrixType::IsT<Throwing>(Format("Index: ",i ," exceeds extents!"))(i<matrix.Cols());
    	    using MDT = MatrixType::DescriptorType;
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e;
    	    std::array<size_t,MatrixType::Order> s;
    	    std::copy(matrix.descriptor.Extents().begin(), matrix.descriptor.Extents().end(), e.begin());
    	    std::copy(matrix.descriptor.Strides().begin(), matrix.descriptor.Strides().end(), s.begin());
    	    for(int i = 0; i < matrix.Rows(); ++i)
    	    {
    	        auto row = matrix.row(i);
				if(*row[i] < 5)
    	        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
    	    }
    	    
    	    e[0] = result.size() / matrix.Cols();
    	    
			return MatrixType(MDT{e,s}, result);
        }
	}
private:
	MatrixType matrix;
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFilter& me) { return s;  }
};
