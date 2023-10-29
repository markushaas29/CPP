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
	template<typename V>     
    V* To() const   { return dynamic_cast<V*>(const_cast<IMatrixCategory*>(this));    }
private:
	friend std::ostream& operator<<(std::ostream& s, const IMatrixCategory& me) { return me.display(s);  }
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<typename T>
class IMatrixStateCategory
{
public:
	using ElementType = T;
	virtual bool operator()() const = 0;
	virtual bool Reset() = 0;
};

template<typename T, typename D>
class CategoryBase : public IMatrixCategory<T>
{
	using Base = IMatrixCategory<T>;
	using Derived = D;
protected:
	template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>
	CategoryBase(TT e): element(e) {}
	CategoryBase(const std::string& s): element(To<typename Base::ElementType>(s)) {}
	Base::ElementType element;
private:
	virtual std::ostream& display(std::ostream& s) const { return s<<Derived::TypeId<<": "<<element; }
};

template<typename T = std::string>
class EquivalenceCat : public CategoryBase<T,EquivalenceCat<T>>
{
	using Base = CategoryBase<T,EquivalenceCat<T>>;
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
class HasCat : public CategoryBase<T,HasCat<T>>
{
	using Base = CategoryBase<T,HasCat<T>>;
public:
	inline static constexpr const char TypeIdentifier[] = "Has";
    inline static constexpr Literal TypeId{TypeIdentifier};

	template<typename TT, typename = typename std::enable_if<!std::is_same<TT, std::string>::value>::type>
	HasCat(TT e): Base(e) {}
	HasCat(const std::string& s): Base(s) {}
	virtual bool operator()(const Base::ElementType& e) const { return String_::Contains(e,Base::element); };
private:
	template<typename U> using IsT =  Is<U,TypeId>;
};
