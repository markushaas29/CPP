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
class IMatrixCategory
{
public:
	using ElementType = T;
	virtual bool operator()(const T& e) const = 0;
};

template<typename T>
class EquivalenceCategory : public IMatrixCategory<T>
{
	using Base = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixCategory";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	EquivalenceCategory(typename Base::ElementType e): element(e) {}
	
	virtual bool operator()(const Base::ElementType& e) const { return e==element; };
private:
	Base::ElementType element;
	template<typename U> using IsT =  Is<U,LiteralType>;
	//friend std::ostream& operator<<(std::ostream& s, const MatrixStrategy& me) { return s<<me.matrix;  }
};

