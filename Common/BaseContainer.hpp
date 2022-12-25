#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/Elements.hpp"

#pragma once
	
	
template<typename Derived, typename T>
class BaseContainer
{
public:
	using Type = Derived;
	using ContainerType = std::vector<T>;
	using ContainerPtr = std::unique_ptr<ContainerType>;
	using TypePtr = T;
	using Iterator = ContainerType::const_iterator;

	const Iterator CBegin() const { return elemnts->cbegin(); }
	const Iterator CEnd() const { return elemnts->cend(); }
	const size_t Size() const { return elemnts->size(); }
	void Add(T t)
	{ 
		using Comparer = typename Derived::Comparer;
		this->elemnts->push_back(t); 
		//~ sortBy(Comparer());
	}
protected:		
	const Iterator begin() const { return elemnts->begin(); }
	const Iterator end() const { return elemnts->end(); }
	BaseContainer(ContainerPtr c): elemnts(std::move(c)){ }
	BaseContainer(): elemnts{std::make_unique<ContainerType>()}{}
	
	std::ostream& Display(std::ostream& os) const {	std::for_each(elemnts->cbegin(), elemnts->cend(), [&](auto const& t){ os<<*t<<std::endl; } ); return os;	}		
private:
	template<typename Comp>
	void sortBy(Comp comp){ std::sort(elemnts->begin(), elemnts->end(), comp); };
	ContainerPtr elemnts;
};

template<typename Derived,typename T>
std::ostream& operator<<(std::ostream& strm, const BaseContainer<Derived,T>& c){	return c.Display(strm);}
