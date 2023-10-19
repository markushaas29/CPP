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
	decltype(auto) FactoryHandle() { return factory; }
    decltype(auto) Unit() { return unit; }
private:
    std::shared_ptr<MultiFactoryType> factory;
    MultiCatUnit<ET> unit;
};

template<typename T, typename ET = T::ElementType>
class MatrixQuery 
{
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixQuery";
    inline static constexpr Literal TypeId{TypeIdentifier};
	MatrixQuery(std::vector<MatrixQueryUnit<T,ET>> units) {}
private:
//	virtual void exec(std::vector<typename Base::MatrixType::DataType>& result, const std::vector<typename Base::MatrixType::DataType>& row, IMatrixCategory<ET>& cat) const
//	{
//    	for(int i = 0; i < row.size(); ++i)
//			if(cat(*row[i]))
//    			std::for_each(row.begin(), row.end(), [&](auto e){ result.push_back(e); });
//	};
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId; };
};
