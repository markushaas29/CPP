#include <vector>
#include <initializer_list>
#include <map>
#include <memory>
#include <sstream>
#include "../Decorator/ElementDecorator.hpp"

#pragma once

struct German;

template<typename M, typename L = German>
class MatrixFormatter 
{
	using MapType  =std::map<std::string, std::string>;
public:
	MatrixFormatter(const M& m): matrix(m), translate{read()}{}
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
    virtual std::string operator()() {	return "<!doctype html>" + Html()(table()).Data();	};
    virtual std::shared_ptr<std::ofstream> operator()(const std::string& n, const std::string& p = "") 
	{	
		auto out = std::make_shared<std::ofstream>(p + "/" + n);
		(*out)<<(*this)();	
		return out;	
	};
    virtual std::ofstream& operator()(std::ofstream& s) 
	{	
		s<<table();	
		return s;	
	};
    auto As()
    {
        auto v = std::vector<typename M::ElementType>();
        std::for_each(matrix.elements->begin(),matrix.elements->end(), [&](auto i) 
				{ 
					if constexpr(std::is_same_v<typename M::ElementType, std::shared_ptr<IElement>>)
						v.push_back(translate->contains((**i).Data()) ? (**i).Clone(translate->at((**i).Data())) : *i); 
					else
						v.push_back(translate->contains(*i) ? (*translate)[*i] : *i); 
				});
		auto d = typename M::DescriptorType(matrix.descriptor.Extents(), matrix.descriptor.Strides());
        return M(d,v);
    }
private:
	const M matrix;
	std::unique_ptr<MapType> translate;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFormatter& m) { return s<<m.table(); }
    virtual std::string table() const 	{	return Table()(rows(matrix)).Data();	};
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
				res+=td(b(is.str())).Data()+"\n";
			}
			return Tr()(res).Data();
		}
		else
		{
			std::string res;
			for(auto i = 0; i != m.Rows(); ++i)
				res += rows(m[i]);
			return res;
		}
	};
	static auto read()
	{
		auto m = std::make_unique<MapType>();
		(*m)["SumPerArea"] = "Summe";
		(*m)["Stage"] = "Stock";
		return m;
	}
};

template<typename M>
MatrixFormatter(const M&) -> MatrixFormatter<M>;
