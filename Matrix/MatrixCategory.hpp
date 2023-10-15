#include <memory>
#include <tuple>
#include <vector>
#include <string_view>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "../Is/Is.hpp"
#include "../To/To.hpp"
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
	template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>
	CategoryBase(TT e): element(e) {}
	CategoryBase(const std::string& s): element(To<typename Base::ElementType>(s)) {}
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

	template<typename TT, typename = typename std::enable_if<std::is_same<TT, T>::value>::type>
	EquivalenceCat(TT e): Base(e) {}
	EquivalenceCat(const std::string& s): Base(s) {}
	virtual bool operator()(const Base::ElementType& e) const { return e==Base::element; };
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};

template<typename T = std::string>
class ContainCat : public CategoryBase<T>
{
	using Base = CategoryBase<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Contain";
    inline static constexpr Literal TypeId{TypeIdentifier};

	template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>
	ContainCat(TT e): Base(e) {}
	ContainCat(const std::string& s): Base(s) {}
	virtual bool operator()(const Base::ElementType& e) const { return String_::Contains(e,Base::element); };
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};
