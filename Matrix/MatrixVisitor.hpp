 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"
 #include "../Visitor/VisitorCollector.hpp"

#pragma once 

template<typename, typename> class M3;

template<typename T>
class MatrixVisitor
{
public:
	using VisitorType = T;
	using CollectorType = VisitorCollector<VisitorType>;
	template<typename M>
	decltype(auto) Visit(M* m) const
	{
		if constexpr (M::Order==1)
		{
			VisitorType v;
			for(auto i=0; i<m->Rows(); ++i)
				(*m->elements->at(i))->Accept(v);

			collector.Add(v);
		}
		else
		{
			for(auto i = 0; i != m->Rows(); ++i)
				(*m)[i].Accept(*this);
		}
	};
	decltype(auto) Collector() const { return collector; }
	template<typename P>
	decltype(auto) All() const 	{	return collector.template All<P>();	}
	template<typename P>
	decltype(auto) Sort() const {	return collector.template Sort<P>(); }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixVisitor& m)  { return s<<m.collector; }
	CollectorType collector;
};
