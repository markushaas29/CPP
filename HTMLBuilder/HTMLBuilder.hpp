#include <vector>
#include <initializer_list>
#include <map>
#include <memory>
#include <sstream>
#include "../Decorator/ElementDecorator.hpp"

#pragma once

struct German;

template<typename L = German>
class HTMLBuilder 
{
	using MapType  =std::map<std::string, std::string>;
public:
	HTMLBuilder(const std::string& n, const std::string& p = ""): name(n), path{p}, translate{read()}, f{std::make_unique<std::ofstream>(path + "/" + name)}
	{
		*f<<"<!doctype html>";
		*f<<"<html>";
	}
    virtual std::string operator()() 
	{	
		*f<<"</html>";	
		return "";
	};
    virtual std::ofstream& operator()(std::ofstream& s) 
	{	
		return s;	
	};
    auto As()
    {
//        auto v = std::vector<typename M::ElementType>();
//        std::for_each(matrix.elements->begin(),matrix.elements->end(), [&](auto i) 
//				{ 
//					if constexpr(std::is_same_v<typename M::ElementType, std::shared_ptr<IElement>>)
//						v.push_back(translate->contains((**i).Data()) ? (**i).Clone(translate->at((**i).Data())) : *i); 
//					else
//						v.push_back(translate->contains(*i) ? (*translate)[*i] : *i); 
//				});
//		auto d = typename M::DescriptorType(matrix.descriptor.Extents(), matrix.descriptor.Strides());
//        return M(d,v);
    }
private:
	const std::string name;
	const std::string path;
	std::unique_ptr<std::ofstream> f;
	std::unique_ptr<MapType> translate;
	friend std::ostream& operator<<(std::ostream& s, const HTMLBuilder& m) 
	{ 
		std::stringstream ss;
		ss<<m.f->rdbuf();
		return s<<ss.str(); }
	static auto read()
	{
		auto m = std::make_unique<MapType>();
		(*m)["SumPerArea"] = "Summe";
		(*m)["Stage"] = "Stock";
		return m;
	}
};

template<typename M>
HTMLBuilder(const M&) -> HTMLBuilder<M>;
