#include <vector>
#include <initializer_list>
#include <map>
#include <memory>
#include <sstream>
#include "../Decorator/ElementDecorator.hpp"
#include "../CSV/IModel.hpp"
#include "../CSV/IHtml.hpp"

#pragma once

struct German;

template<typename M, typename L = German>
class MatrixFormatter: public IModel, public IHtml
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
	virtual std::unique_ptr<IModel> Model() const { return std::make_unique<MatrixFormatter<M,L>>(matrix); }
    virtual std::string operator()() {	return table();	};
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
						v.push_back(translate->contains((**i).Data()) ? (**i).Create(translate->at((**i).Data())) : *i); 
					else
						v.push_back(translate->contains(*i) ? (*translate)[*i] : *i); 
				});
		auto d = typename M::DescriptorType(matrix.descriptor.Extents(), matrix.descriptor.Strides());
        return M(d,v);
    }
private:
	const M matrix;
	std::shared_ptr<MapType> translate;
	friend std::ostream& operator<<(std::ostream& s, const MatrixFormatter& m) { return s<<m.table(); }
    virtual std::string table() const 	{	return HtmlElement<Entry,Table>(rows(matrix)).Data();	};
	virtual std::string out(const std::string& intent, uint i = 0) const  { return table(); };
    virtual std::unique_ptr<IHtmlElement> html() const 	{	return std::make_unique<HtmlElement<Entry,Table>>(rows(matrix));	};
	template<size_t O, typename D>
    std::string rows(const Matrix<O,D>& m) const
	{ 
		if constexpr (O==1)
		{
			std::string res;
			for(auto i=0; i<m.Rows(); ++i)
			{
				std::stringstream is;
				if constexpr(std::is_same_v<typename M::ElementType, std::shared_ptr<IElement>>)
					is<<(*(*(m.elements->at(i)))->Html());
				else
					is<<HtmlElement<Entry,Td>(is.str()).Data();
				res+=is.str()+"\n";
			}
			return HtmlElement<Entry,Tr>(res).Data();
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
		auto m = std::make_shared<MapType>();
		(*m)["SumPerArea"] = "Summe";
		(*m)["Stage"] = "Stock";
		return m;
	}
};

template<typename M>
MatrixFormatter(const M&) -> MatrixFormatter<M>;
