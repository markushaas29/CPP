#include <map>
#include <memory>
#include <sstream>
#include "../CSV/HtmlElement.hpp"
#include "../CSV/Elements.hpp"

#pragma once

struct German;

template<typename L = German>
class HtmlBuilder 
{
	using MapType  =std::map<std::string, std::string>;
public:
HtmlBuilder(const std::string& n = "", const std::string& p = ""): name(n), path{p}, translate{read()}, f{std::make_unique<std::ofstream>(path + "/" + name)}
	{
		*f<<"<!doctype html>";
		*f<<"<html>";
	}
    auto operator()() 
	{	
		*f<<"</html>";	
		f->close();
	};
    std::ofstream& operator()(std::ofstream& s, const auto& a) const
	{	
		s<<a;
		return s;	
	};
    void operator()(const std::string& a) const 	{	*f<< HtmlElement<Entry,Section>(a).Data();	};
    void operator()(const auto& a) const	{	*f<< HtmlElement<decltype(a),Section>(a).Data();};
private:
	const std::string name;
	const std::string path;
	std::unique_ptr<std::ofstream> f;
	std::unique_ptr<MapType> translate;
	friend std::ostream& operator<<(std::ostream& s, const HtmlBuilder& m) 
	{ 
		std::stringstream ss;
		ss<<m.f->rdbuf();
		return s<<ss.str(); 
	}
	static auto read()
	{
		auto m = std::make_unique<MapType>();
		(*m)["SumPerArea"] = "Summe";
		(*m)["Stage"] = "Stock";
		return m;
	}
};

template<typename M>
HtmlBuilder(const M&) -> HtmlBuilder<M>;
