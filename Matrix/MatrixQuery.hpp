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


template<typename T, typename ET>
class IMatrixQuery
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	using CategoryType = IMatrixCategory<ET>;
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	MatrixType operator()( MatrixType* matrix) const
	{
		if constexpr (MatrixType::Order==2)
        {
    	    std::vector<typename MatrixType::DataType> result;
    	    std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());

    	    for(int j = 0; j < matrix->Rows(); ++j)
    	        exec(result, matrix->row(j), *cat);


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
protected:
	IMatrixQuery(std::unique_ptr<CategoryType> c): cat{std::move(c)} {}
private:
	std::unique_ptr<CategoryType> cat;
	virtual void exec(std::vector<typename MatrixType::DataType>& result, const std::vector<typename MatrixType::DataType>& row, const IMatrixCategory<ET>& cat) const = 0;
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

template<typename T, typename ET = T::ElementType>
class MatrixQuery:public IMatrixQuery<T,ET> 
{
	using Base = IMatrixQuery<T,ET>;
public:
	MatrixQuery(std::unique_ptr<typename Base::CategoryType> c): Base{std::move(c)}{}
private:
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, const IMatrixCategory<ET>& cat) const
	{
    	for(int i = 0; i < row.size(); ++i)
			if(cat(*row[i]))
    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
};

template<typename T, typename ET = T::ElementType>
class MatrixColQuery: public IMatrixQuery<T,ET> 
{
	using Base = IMatrixQuery<T,ET>;
public:
	MatrixColQuery(size_t c, std::unique_ptr<typename Base::CategoryType> cat): Base{std::move(cat)}, col{c} {}
private:
	size_t col;
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, const IMatrixCategory<ET>& cat) const
	{
		if(cat(*row[col]))
        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
};
