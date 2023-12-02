 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"
 #include "../Visitor/VisitorCollector.hpp"

#pragma once 

template<typename T>
class MatrixVisitor
{
		struct Item
		{
			IBAN Key;
			std::vector<T> Value;
		};
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
	decltype(auto) Sort() const 
	{
		auto v = collector.IBANS();
		MatrixDescriptor<2,std::shared_ptr<IElement>> md{{v.size()/3,3}};
	 	return Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>(md,v);
	}
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixVisitor& m)  { return s<<m.collector; }
	CollectorType collector;
};
