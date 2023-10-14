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

#pragma once

template<std::size_t, typename> class Matrix;

template<typename T>
class MultiCat : public IMatrixCategory<T>
{
	using Base = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Multi";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MultiCat(std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> e): elements(std::move(e)) {}
	virtual bool operator()(const Base::ElementType& e) const 
	{ 
		for(auto i = 0; i < elements->size(); ++i)
			if((*(elements->at(i)))(e))
				return true; 

		return false;
	};
	decltype(auto) Size() const { return elements->size(); };
private:
	std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> elements;
	template<typename U> using IsT =  Is<U,TypeId>;
	virtual std::ostream& display(std::ostream& s) const 
	{ 
		s<<TypeId<<": "; 
		std::for_each(elements->cbegin(), elements->cend(), [&s](auto &e) { s<<*e; });
		return s;
	}
};

template<typename T>
class MultiStateCat : public IMatrixStateCategory<T>
{
	using Base = IMatrixCategory<T>;
public:
	inline static constexpr const char TypeIdentifier[] = "Multi";
    inline static constexpr Literal TypeId{TypeIdentifier};

	MultiStateCat(std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> e): elements(std::move(e)), states{std::make_unique<std::vector<size_t>>(elements->size(),0)} {}
	virtual bool operator()(const Base::ElementType& e) const 
	{ 
		for(auto i = 0; i < elements->size(); ++i)
			if((*(elements->at(i)))(e))
				++(states->at(i)); 

		return false;
	};
	virtual bool operator()() const { return all_of(states->begin(), states->end(), [] (size_t i) {return i > 0;}); };
	virtual bool Reset() 
	{ 
		states = std::make_unique<std::vector<size_t>>(elements->size(),0);
		return true; 
	};
	decltype(auto) Size() const { return elements->size(); };
private:
	std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<T>>>> elements;
	std::unique_ptr<std::vector<size_t>> states;
	template<typename U> using IsT =  Is<U,TypeId>;
	virtual std::ostream& display(std::ostream& s) const 
	{ 
		s<<TypeId<<": "; 
		std::for_each(elements->cbegin(), elements->cend(), [&s](auto &e) { s<<*e; });
		return s;
	}
};
