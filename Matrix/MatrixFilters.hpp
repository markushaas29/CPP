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

template<typename T>
class UniqueElements 
{
public:
	using MatrixType = T;
	inline static constexpr const char TypeIdentifier[] = "MatrixFilter_UniqueElement";
    inline static constexpr Literal LiteralType{TypeIdentifier};

	//decltype(auto) operator()() const { return elements.erase(std::unique(elements.begin(), elements.end()),elements.end()); } 
	decltype(auto) operator()() const { return elements; } 
	decltype(auto) operator()(const typename MatrixType::ElementType& e) 
	{
		std::cout<<"E "<<e<<std::endl;
		if(std::find(elements.begin(), elements.end(), e) != elements.end()) 
			return false;
		elements.push_back(e);
		return true;
	} 
private:
	std::vector<typename MatrixType::ElementType> elements;
	template<typename U> using IsT =  Is<U,LiteralType>;
	friend std::ostream& operator<<(std::ostream& s, const UniqueElements& me) 
	{ 
		std::for_each(me.elements.cbegin(), me.elements.cend(), [&](const auto& e) { s<<e<<"\n"; });
		return s;
	}
};
