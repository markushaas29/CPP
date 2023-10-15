#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/String_.hpp"
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
private:
	friend std::ostream& operator<<(std::ostream& s, const IMatrixCategory& me) { return me.display(s);  }
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<typename T>
class IMatrixStateCategory: public IMatrixCategory<T> 
{
public:
	virtual bool operator()() const = 0;
	virtual bool Reset() = 0;
};

template<typename T>
class CategoryBase : public IMatrixCategory<T>
{
	using Base = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Equivalence";
    inline static constexpr Literal TypeId{TypeIdentifier};
protected:
	CategoryBase(typename Base::ElementType e): element(e) {}
	Base::ElementType element;
private:
	virtual std::ostream& display(std::ostream& s) const { return s<<TypeId<<": "<<element; }
};

template<typename T>
class EquivalenceCat : public CategoryBase<T>
{
	using Base = CategoryBase<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Equivalence";
    inline static constexpr Literal TypeId{TypeIdentifier};

	EquivalenceCat(typename Base::ElementType e): Base(e) {}
	virtual bool operator()(const Base::ElementType& e) const { return e==Base::element; };
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};

template<typename T>
class ContainCat : public CategoryBase<T>
{
	using Base = CategoryBase<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Contain";
    inline static constexpr Literal TypeId{TypeIdentifier};

	ContainCat(typename Base::ElementType e): Base(e) {}
	virtual bool operator()(const Base::ElementType& e) const { return String_::Contains(e,Base::element); };
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};
