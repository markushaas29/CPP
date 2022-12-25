#include <memory>
#include <vector>
#include <algorithm>    // std::find
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Algorithms.hpp"
#include "../String/String_.hpp"
#include "../CSV/Elements.hpp"

#pragma once
	
	
template<typename T>
class BaseContainer
{
public:
	using Type = BaseContainer<T> ;
	using ContainerType = std::vector<T>;
	using ContainerPtr = std::unique_ptr<ContainerType>;
	using TypePtr = T;
	using Iterator = ContainerType::const_iterator;

	BaseContainer(): elemnts{std::make_unique<ContainerType>()}{}

	const Iterator CBegin() const { return elemnts->cbegin(); }
	const Iterator CEnd() const { return elemnts->cend(); }
	const size_t Size() const { return elemnts->size(); }
	void Add(T t)
	{ 
		Logger::Log<Error>("Base");
		this->elemnts->push_back(t); 
		std::sort(elemnts->begin(), elemnts->end(), [](const auto& r1, const auto& r2){ return r1->Date > r2->Date; });
	}
protected:		
	const Iterator begin() const { return elemnts->begin(); }
	const Iterator end() const { return elemnts->end(); }
	
	std::ostream& Display(std::ostream& os) const {	std::for_each(elemnts->cbegin(), elemnts->cend(), [&](auto const& t){ os<<*t<<std::endl; } ); return os;	}		
private:
	ContainerPtr elemnts;
	BaseContainer(ContainerPtr c): elemnts(c){ }
};

template<typename T>
std::ostream& operator<<(std::ostream& strm, const BaseContainer<T>& c){	return c.Display(strm);}
