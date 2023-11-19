 #include "../Visitor/Visitor.hpp"
 #include "../Visitor/ElementVisitor.hpp"

#pragma once 

template<typename T>
class MatrixVisitor
{
public:
	using VisitorType = T;
	template<typename M>
	decltype(auto) Visit(M* m) const
	{
		if constexpr (M::Order==1)
		{
			VisitorType v;
			for(auto i=0; i<m->Rows(); ++i)
				(*m->elements->at(i))->Accept(v);

			std::cout<<v<<std::endl;
		}
		else
		{
			for(auto i = 0; i != m->Rows(); ++i)
				(*m)[i].Accept(*this);
		}
	};
};
