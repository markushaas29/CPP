#include <vector>
#include <initializer_list>
#include <memory>
#include "PointerConcept.hpp"
#include "../Components/Interfaces.hpp"

#pragma once

template<typename M>
class MatrixIO: IOut
{
public:
	MatrixIO() {}
    virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> s) { return s; }
    virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> s, const M* m)
	{ 
		if constexpr (M::Order==1)
		{
			*s<<"{";
			for(auto i=0; i<m->Rows(); ++i)
				if constexpr (PointerConcept<decltype(*(m->elements->at(i)))>)
					*s<<*(*(m->elements->at(i)))<<(i+1 == m->Rows()? "" : ", ");
				else
					*s<<*(m->elements->at(i))<<(i+1 == m->Rows()? "" : ", ");
			return s;
		}
		else
		{
			*s<<"{\n";
			for(auto i = 0; i != m->Rows(); ++i)
				*s<<(*m)[i]<<std::endl;
			*s<<"}";
		}
		return s;
	};
	virtual std::ostream& operator()(std::ostream& s) {	return s;	}
    virtual std::ostream& operator()(std::ostream& s, const M* m) 
	{ 
		if constexpr (M::Order==1)
		{
			s<<"{";
			for(auto i=0; i<m->Rows(); ++i)
				if constexpr (PointerConcept<decltype(*(m->elements->at(i)))>)
					s<<*(*(m->elements->at(i)))<<(i+1 == m->Rows()? "" : ", ");
				else
					s<<*(m->elements->at(i))<<(i+1 == m->Rows()? "" : ", ");
			return s<<" }";
		}
		else
		{
			s<<"{\n";
			for(auto i = 0; i != m->Rows(); ++i)
				in(s)<<(*m)[i]<<std::endl;
			s<<"}";
		}
		return s;
	};
private:
	friend M;
    std::ostream& in(std::ostream& s)
	{
		for(auto i=0; i<M::Order; ++i)
			s<<" ";
		return s;
	}
};
