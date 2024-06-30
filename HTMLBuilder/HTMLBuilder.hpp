
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
	HTMLBuilder(const std::string& n = "", const std::string& p = ""): name(n), path{p}, translate{read()}, f{std::make_shared<std::ofstream>(path + "/" + name)}
	{
		*f<<"<!doctype html>";
		*f<<"<html>";
	}
    auto operator()() 
	{	
		*f<<"</html>";	
		f->close();
	};
    std::ofstream& operator()(std::ofstream& s, const auto& a) 
	{
		s<<a;
		return s;	
	};
    void operator()(const auto& a) 
	{	
		*f<<Section()(a).Data();
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
	auto Of() { return f; }
private:
	const std::string name;
	const std::string path;
	std::shared_ptr<std::ofstream> f;
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
