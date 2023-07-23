#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "../Components/Interfaces.hpp"

#pragma once

template<typename M>
class MatrixIO: IOut
{
public:
	MatrixIO(M* ma): m{ma} {}
    virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> of) { return of; };
	virtual std::ostream& operator()(std::ostream& s) 
	{
		if constexpr (M::Order==1)
		{
			std::for_each(m->elements->cbegin(), m->elements->cend(), [&](auto& v) { s<<*v<<", "; });
			s<<"\nExpressions:\n";
			std::for_each(m->expressions->cbegin(), m->expressions->cend(), [&](auto& v) { s<<*v<<", "; });
			return s;
		}
		else
		{
			for(auto i = 0; i != m->Rows(); ++i)
				s<<"{"<<(*m)[i]<<"}"<<std::endl;
		}
		return s;
	}
private:
	M* m;
	friend M;
};
