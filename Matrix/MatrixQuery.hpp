#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixCategory.hpp"
#include "IMatrixQuery.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../Common/UniqueCast.hpp"
#include "../ObjectFactory/Factory.hpp"

#pragma once

template<typename T, typename ET = T::ElementType>
class MatrixQuery: public IMatrixQuery<T,ET> 
{
	using MatrixType = T;
	using ElementType = T::ElementType;
	using CategoryType = IMatrixCategory<ET>;
	using FactoryType = FactoryStack<CategoryType, Factory<CategoryType>>;
	using ArgType = FactoryUnit<typename FactoryType::IdentifierType, typename FactoryType::ArgumentType>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixQuery(std::shared_ptr<FactoryType> f, const std::vector<ArgType>& u): factory{f}, cats{createCats(u)}, units{u} {}
 	virtual	MatrixType operator()( MatrixType* matrix) const
	{
		std::vector<typename MatrixType::DataType> result;
    	std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());
		
		if constexpr (MatrixType::Order==2)
        {
    	    for(int j = 0; j < matrix->Rows(); ++j)
			{
				auto c = createCats(units);
   				for(int i = 0; i < c.size(); ++i)
   				{
	   				  MatrixRowQuery<T,ET> rq{std::move(c.at(i))};;
	   				  if(rq(matrix->row(j)))
	   				  {
	   				  		auto row = matrix->row(j);
	   				  		std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	   				  		break;
	   				  }
   				}
			}


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
	std::shared_ptr<FactoryType> factory;
	std::vector<ArgType> units;
	std::vector<std::unique_ptr<CategoryType>> cats;
	decltype(auto) createCats(const auto& mus) const
	{
		std::vector<std::unique_ptr<CategoryType>> res;
		std::for_each(mus.cbegin(), mus.cend(),[&](const auto& mu) { res.push_back(std::move((*factory)( mu.Id(), mu.Arg()))); });
		return res;
	}
	template<size_t N>
	decltype(auto) copy(std::array<size_t,N> arr) const
	{
    	std::array<size_t,MatrixType::Order> res;
		std::copy(arr.begin(), arr.end(), res.begin());
		return res;
	}
	friend std::ostream& operator<<(std::ostream& s, const MatrixQuery& mq) { return mq.display(s); }
	virtual std::ostream& display(std::ostream& s) const
	{
		s<<TypeId<<"\n";
		std::for_each(cats.cbegin(), cats.cend(),[&](const auto& mu) { s<<"\t"<<*mu<<"\n"; });
		return s;
	};
};
