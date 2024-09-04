#include "Style.hpp"
#include <vector>

#pragma once

class ICss
{
public:
	virtual std::string operator()() const  { return data(); };	
	virtual std::unique_ptr<ICss> Clone() const  = 0;	
private:
	friend std::ostream& operator<<(std::ostream& out, const ICss& e) {	return out<<e.data();}
	virtual std::string data() const  = 0;	
};
////--------------------------------Css------------------------------------------------

template<typename... T>
class Css: public ICss
{
protected:
	using Tup = std::tuple<T...>;
	template<typename...> friend class Css;
	std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> styles;
	virtual std::string data() const  
	{	
		std::string res = " style=\"";
		std::for_each(styles->cbegin(), styles->cend(), [&](const auto& s) { res += (*s)(); });
		return res + "\"";
	};	
	static std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> create()
	{
		auto res = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
		return exec<0>(res);
	}
	template<size_t N>
    static auto exec(auto&& res)
    {
        if constexpr (std::tuple_size<Tup>()==N)
            return std::move(res);
        else
        {
            using Type = std::tuple_element_t<N,Tup>;
            res->push_back(std::make_unique<Type>());
            return exec<N+1>(res);
        }
    }
public:
 	Css(const std::string& c = ""): styles{create()} { };
//	template<typename... T2>
//	auto operator()(const Css<T2...>& css)
//	{
//		std::for_each(css.styles->cbegin(), css.styles->cend(), [&](auto& p)
//				{
//				 	auto it = std::find_if(styles->begin(), styles->end(), [&] (auto& p2) { return p->Element() == p2->Element(); } );
//				 	if(it != styles->end())
//						*it = (p->Clone());
//				});
//
//	}
	virtual std::unique_ptr<ICss> Clone() const  { return std::make_unique<Css>(); };	
};

template<typename... T>
class ClassCss: public Css<T...>
{
	using Base = Css<T...>;
public:
 	ClassCss(const std::string& c): Base{}, name{c} { };
private:
	std::string name;
	virtual std::string data() const  
	{	
		std::string res = "." + name + " {\n";
		std::for_each(Base::styles->cbegin(), Base::styles->cend(), [&](const auto& s) { res += (*s)(); });
		return res + "\n}";
	};	
};
