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
    virtual std::string operator()() 
	{ 
		return "<!doctype html>" + Html()(table());
	};
private:
	const M matrix;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFormatter& m) { return s<<m.table(); }
    virtual std::string table() const
	{
		return Table()(rows(matrix));
	};
	template<size_t O, typename D>
    std::string rows(const Matrix<O,D>& m) const
	{ 
		if constexpr (O==1)
		{
			auto td = Td();
			auto b = B();
			std::string res;
			for(auto i=0; i<m.Rows(); ++i)
			{
				std::stringstream is;
				is<<m[i];
				res+=td(b(is.str()))+"\n";
			}
			return Tr()(res);
		}
		else
		{
			std::string res;
			for(auto i = 0; i != m.Rows(); ++i)
				res += rows(m[i]);
			return res;
		}
	};
};

template<typename M>
MatrixFormatter(const M&) -> MatrixFormatter<M>;
