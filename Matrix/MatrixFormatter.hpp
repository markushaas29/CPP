#include <vector>
#include <initializer_list>
#include <memory>
#include <sstream>
#include "../Decorator/ElementDecorator.hpp"

#pragma once

template<typename M>
class MatrixFormatter 
{
public:
	MatrixFormatter(const M& m): matrix(m){}
   // virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> s) { return s; }
//    virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> s, const M* m) 
//	{ 
//		if constexpr (M::Order==1)
//		{
//			for(auto i=0; i<m->Rows(); ++i)
//				if constexpr (PointerConcept<decltype(*(m->elements->at(i)))>)
//					*s<<*(*(m->elements->at(i)))<<(i+1 == m->Rows()? "" : ", ");
//				else
//					*s<<*(m->elements->at(i))<<(i+1 == m->Rows()? "" : ", ");
//			return s;
//		}
//		else
//		{
//			for(auto i = 0; i != m->Rows(); ++i)
//			{
//				s = (*m)[i].file(std::move(s));
//				*s<<std::endl;
//			}
//		}
//		return s;
//	};
//	virtual std::ostream& operator()(std::ostream& s) {	return s;	}
//    virtual std::ostream& operator()(std::ostream& s, const M* m) 
//	{ 
//		if constexpr (M::Order==1)
//		{
//			s<<"{";
//			for(auto i=0; i<m->Rows(); ++i)
//				if constexpr (PointerConcept<decltype(*(m->elements->at(i)))>)
//					s<<*(*(m->elements->at(i)))<<(i+1 == m->Rows()? "" : ", ");
//				else
//					s<<*(m->elements->at(i))<<(i+1 == m->Rows()? "" : ", ");
//			return s<<" }";
//		}
//		else
//		{
//			s<<"{\n";
//			for(auto i = 0; i != m->Rows(); ++i)
//				in(s)<<(*m)[i]<<std::endl;
//			s<<"}";
//		}
//		return s;
//	};
private:
	const M matrix;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFormatter& m) { return m(s); }
    virtual std::ostream& operator()(std::ostream& s) const
	{ 
		if constexpr (M::Order==1)
		{
			auto b = Td();
			std::string res;
			for(auto i=0; i<matrix.Rows(); ++i)
			{
				std::stringstream is;
				is<<matrix[i];
				res+=b(is.str())+"\n";
			}
			return s<<Tr()(res);
		}
		else
		{
			s<<"{\n";
//			for(auto i = 0; i != m->Rows(); ++i)
				//in(s)<<(*m)[i]<<std::endl;
			s<<"}";
		}
		return s;
	};
};

template<typename M>
MatrixFormatter(const M&) -> MatrixFormatter<M>;
