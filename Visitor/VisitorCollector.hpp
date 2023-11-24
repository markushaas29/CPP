 #include <vector>
 #include <memory>
 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"

#pragma once 

template<typename T>
class VisitorCollector
{
public:
	using VisitorType = T;
	VisitorCollector(): visitors(std::make_unique<std::vector<VisitorType>>()) { }
	decltype(auto) Add(const VisitorType& v) const { visitors->push_back(v); }
private:
	friend std::ostream& operator<<(std::ostream& s, const VisitorCollector& t)  
	{
		std::for_each(t.visitors->cbegin(), t.visitors->cend(), [&](auto& v) { s<<v<<"\n"; });
		return s; 
	}
	std::unique_ptr<std::vector<VisitorType>> visitors;
};
