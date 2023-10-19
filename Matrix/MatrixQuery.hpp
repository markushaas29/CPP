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
#include "../Common/UniqueCast.hpp"
#include "../ObjectFactory/Factory.hpp"

#pragma once
template<typename T> class MultiCatUnit;

template<typename T, typename ET>
class MatrixQueryUnit
{
	using MultiFactoryType = IFactory<IMatrixCategory<ET>, MultiCatUnit<ET>>;
public:
	MatrixQueryUnit(std::shared_ptr<MultiFactoryType> f, MultiCatUnit<ET> u): factory{f}, unit{u} {}
	decltype(auto) FactoryHandle() const { return factory; }
    decltype(auto) Unit() const { return unit; }
private:
    std::shared_ptr<MultiFactoryType> factory;
    MultiCatUnit<ET> unit;
};

template<typename T, typename ET = T::ElementType>
class MatrixQuery 
{
	using MatrixType = T;
	using ElementType = T::ElementType;
	using CategoryType = IMatrixCategory<ET>;
	using MultiFactoryType = IFactory<CategoryType, MultiCatUnit<ET>>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixQuery(std::shared_ptr<MultiFactoryType> f, std::vector<MatrixQueryUnit<T,ET>> units): factory{f}, cats{createCats(units)} {}
 	virtual	MatrixType operator()( MatrixType* matrix) const
	{
		std::vector<typename MatrixType::DataType> result;
    	std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());
		
		if constexpr (MatrixType::Order==2)
        {
    	    for(int j = 0; j < matrix->Rows(); ++j){}


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
		if constexpr (MatrixType::Order==1)
        {
    	    for(int j = 0; j < matrix->Rows(); ++j)
    	        if((*cats[0])(*(*matrix)(j)))
					result.push_back((*matrix)(j));


    	    e[0] = result.size();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
private:
	std::shared_ptr<MultiFactoryType> factory;
	std::vector<std::unique_ptr<CategoryType>> cats;
	decltype(auto) createCats(const auto& mus) const
	{
		std::vector<std::unique_ptr<CategoryType>> res;
		std::for_each(mus.cbegin(), mus.cend(),[&](const auto& mu) { res.push_back(std::move((*factory)( mu.Unit().Type(), mu.Unit()))); });

		std::cout<<"SIZE"<<res.size();

		return res;
	}
	template<size_t N>
	decltype(auto) copy(std::array<size_t,N> arr) const
	{
    	std::array<size_t,MatrixType::Order> res;
		std::copy(arr.begin(), arr.end(), res.begin());
		return res;
	}
	friend std::ostream& operator<<(std::ostream& s, const MatrixQuery& mq)
	{
		s<<TypeId<<"\n";
		std::for_each(mq.cats.cbegin(), mq.cats.cend(),[&](const auto& mu) { s<<"\t"<<*mu<<"\n"; });
		return s;
	};
//	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const
//	{
//    	for(int i = 0; i < row.size(); ++i)
//			if(cat(*row[i]))
//    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
//	};
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId; };
};
