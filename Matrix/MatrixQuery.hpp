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

template<typename T, typename ET = T::ElementType>
class IMatrixQuery
{
public:
	using MatrixType = T;
	using ElementType = T::ElementType;
	using CategoryType = IMatrixCategory<ET>;
 	virtual	MatrixType operator()( MatrixType* matrix) const
	{
		std::vector<typename MatrixType::DataType> result;
    	std::array<size_t,MatrixType::Order> e = copy(matrix->descriptor.Extents());
		
		if constexpr (MatrixType::Order==2)
        {
    	    for(int j = 0; j < matrix->Rows(); ++j)
    	        exec(result, matrix->row(j), *cat);


    	    e[0] = result.size() / matrix->Cols();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
		if constexpr (MatrixType::Order==1)
        {
    	    for(int j = 0; j < matrix->Rows(); ++j)
    	        if((*cat)(*(*matrix)(j)))
					result.push_back((*matrix)(j));


    	    e[0] = result.size();
    	    
			return MatrixType(typename MatrixType::DescriptorType{e,copy(matrix->descriptor.Strides())}, result);
        }
	}
protected:
	using FactoryType = IFactory<IMatrixCategory<ET>>;
	using MultiFactoryType = IFactory<CategoryType, ET>;
	using UnitType = FactoryUnit<typename FactoryType::IdentifierType, typename FactoryType::ArgumentType>;
	IMatrixQuery(std::unique_ptr<CategoryType> c): cat{std::move(c)} {}
	IMatrixQuery(std::shared_ptr<Factory<CategoryType, MultiCatUnit<ET>>> f, MultiCatUnit<ET> units): factory{f}, cat{std::move((*f)( units.Type(), units))}{ }
private:
	std::shared_ptr<Factory<CategoryType, MultiCatUnit<ET>>> factory;
	std::unique_ptr<CategoryType> cat;
	virtual void exec(std::vector<typename MatrixType::DataType>& result, const std::vector<typename MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const = 0;
	template<size_t N>
	decltype(auto) copy(std::array<size_t,N> arr) const
	{
    	std::array<size_t,MatrixType::Order> res;
		std::copy(arr.begin(), arr.end(), res.begin());
		return res;
	}
	friend std::ostream& operator<<(std::ostream& s, const IMatrixQuery& mq) { return mq.display(s)<<": ["<<(*mq.cat)<<"]";  }
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<typename T, typename ET>
class MatrixQueryBase : public IMatrixQuery<T,ET>
{
	using Base = IMatrixQuery<T,ET>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixQueryBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
protected:
	MatrixQueryBase(std::unique_ptr<typename Base::CategoryType> c): Base{std::move(c)} {}
	MatrixQueryBase(std::shared_ptr<Factory<typename Base::CategoryType, MultiCatUnit<ET>>> f, MultiCatUnit<typename Base::ElementType> units): Base{f, units} {}
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};

template<typename T, typename ET = T::ElementType>
class MatrixQuery: public MatrixQueryBase<T,ET> 
{
	using Base = MatrixQueryBase<T,ET>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixQuery(std::unique_ptr<typename Base::CategoryType> c): Base{std::move(c)}{}
	MatrixQuery(std::shared_ptr<typename Base::MultiFactoryType> f, MultiCatUnit<typename Base::ElementType> units): Base{f, units} {}
private:
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const
	{
    	for(int i = 0; i < row.size(); ++i)
			if(cat(*row[i]))
    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId; };
};

template<typename T, typename ET = T::ElementType>
class MatrixRowQuery: public MatrixQueryBase<T,ET> 
{
	using Base = MatrixQueryBase<T,ET>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixRowQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixRowQuery(std::unique_ptr<typename Base::CategoryType> cat): Base{std::move(cat)} {}
	MatrixRowQuery(std::shared_ptr<Factory<typename Base::CategoryType, MultiCatUnit<ET>>> f, MultiCatUnit<ET> units): Base{f, units} {}
private:
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const
	{
    	for(int i = 0; i < row.size(); ++i)
			if(cat(*row[i]))
			{
    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
				break;
			}

		if(IMatrixStateCategory<ET>* p = dynamic_cast<IMatrixStateCategory<ET>*>(&cat))
			if((*p)())
			{
    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
				p->Reset();
			}
	};
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId; };
};

template<typename T, typename ET = T::ElementType>
class MatrixColQuery: public MatrixQueryBase<T,ET> 
{
	using Base = MatrixQueryBase<T,ET>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixColQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixColQuery(size_t c, std::unique_ptr<typename Base::CategoryType> cat): Base{std::move(cat)}, col{c} {}
	MatrixColQuery(std::shared_ptr<typename Base::MultiFactoryType> f, MultiCatUnit<typename Base::ElementType> units): Base{f, units} {}
private:
	size_t col;
	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const
	{
		if(cat(*row[col]))
        	std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
	};
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId; };
};
