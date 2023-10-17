#include <memory>
#include <tuple>
#include <vector>
#include "MatrixConcept.hpp"
#include "PointerConcept.hpp"
#include "MatrixElement.hpp"
#include "MatrixCategory.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/String_.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#include "../Common/TupleHelper.hpp"
#include "../ObjectFactory/Factory.hpp"

#pragma once

template<std::size_t, typename> class Matrix;

template<typename T>
class MultiCategoryBase 
{
	using Base = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Multi";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MultiCategoryBase(std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> e): elements(std::move(e)) {}
	decltype(auto) Size() const { return elements->size(); };
protected:
	std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> elements;
	template<typename U> using IsT =  Is<U,TypeId>;
};

template<typename T>
class MultiCat : public MultiCategoryBase<T>, public IMatrixCategory<T>
{
	using Base = MultiCategoryBase<T>;
	using I = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Multi";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MultiCat(std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> e): Base(std::move(e)) {}
	virtual bool operator()(const I::ElementType& e) const 
	{ 
		for(auto i = 0; i < Base::elements->size(); ++i)
			if((*(Base::elements->at(i)))(e))
				return true; 

		return false;
	};
private:
	virtual std::ostream& display(std::ostream& s) const 
	{ 
		s<<TypeId<<": "; 
		std::for_each(Base::elements->cbegin(), Base::elements->cend(), [&s](auto &e) { s<<*e; });
		return s;
	}
};

template<typename T>
class MultiStateCat : public MultiCategoryBase<T>, public IMatrixStateCategory<T>
{
	using Base = MultiCategoryBase<T>;
	using I = IMatrixStateCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Multi";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MultiStateCat(std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> e): Base(std::move(e)), states{std::make_unique<std::vector<size_t>>(Base::elements->size(),0)} {}
	virtual bool operator()(const I::ElementType& e) const 
	{ 
		for(auto i = 0; i < Base::elements->size(); ++i)
			if((*(Base::elements->at(i)))(e))
				++(states->at(i)); 

		return false;
	};
	virtual bool operator()() const { return all_of(states->begin(), states->end(), [] (size_t i) {return i > 0;}); };
	virtual bool Reset() 
	{ 
		states = std::make_unique<std::vector<size_t>>(Base::elements->size(),0);
		return true; 
	};
private:
	std::unique_ptr<std::vector<size_t>> states;
	template<typename U> using IsT =  Is<U,TypeId>;
	virtual std::ostream& display(std::ostream& s) const 
	{ 
		s<<TypeId<<": "; 
		std::for_each(Base::elements->cbegin(), Base::elements->cend(), [&s](auto &e) { s<<*e; });
		return s;
	}
};
