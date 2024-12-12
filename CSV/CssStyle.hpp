#include <vector>
#include "Style.hpp"
#include "IModel.hpp"

#pragma once

class ICss: public IModel
{
public:
	virtual std::string operator()() const  { return data(); };	
	virtual std::unique_ptr<ICss> Clone() const  = 0;	
	virtual std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> Styles() const = 0;
	//virtual auto Update(auto) const  = 0;	
private:
	friend std::ostream& operator<<(std::ostream& out, const ICss& e) {	return out<<e.data();}
	virtual std::string out(const std::string& intent, uint i = 0) const  {	return intent + data(intent, i) + "\n"; };
	virtual std::string data(const std::string& intent = "", uint i = 0) const  = 0;	
};

class IClassCss
{
public:
	auto Name() const  { return className(); };	
private:
	virtual const std::string& className() const  = 0;	
};
////--------------------------------Css------------------------------------------------

template<typename... T>
class Css: public ICss
{
protected:
	using Tup = std::tuple<T...>;
	template<typename...> friend class Css;
	std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> styles;
	virtual std::string data(const std::string& intent = "", uint i = 0) const
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
	virtual std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> Styles() const 
	{
		auto result = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
		std::for_each(styles->cbegin(), styles->cend(), [&](auto& p) { result->push_back(p->Clone()); });
		return result;
	}
	template<typename... T2>
	auto update(const Css<T2...>& css)
	{
		std::vector<std::unique_ptr<IStyle>> nStyles;
		std::for_each(css.styles->cbegin(), css.styles->cend(), [&](auto& p)
				{
				 	auto it = std::find_if(styles->begin(), styles->end(), [&] (auto& p2) { return p->Element() == p2->Element(); } );
				 	if(it != styles->end())
						*it = (p->Clone());
					else
						nStyles.push_back(p->Clone());
				});
		std::for_each(nStyles.cbegin(), nStyles.cend(), [&](auto& p) { styles->push_back(p->Clone()); } );
	}
	virtual std::unique_ptr<ICss> Clone() const  { return std::make_unique<Css>(); };	
};

template<typename... T>
class ClassCss: public Css<T...>, virtual public IClassCss
{
	using Base = Css<T...>;
public:
 	ClassCss(const std::string& n): Base{}, name{n} { };
	virtual std::unique_ptr<ICss> Clone() const  { return std::make_unique<ClassCss<T...>>(name); };	
private:
	std::string name;
	virtual const std::string& className() const { return name; };	
	virtual std::string data(const std::string& intent = "", uint i = 0) const
	{
		std::string res = "." + name + " {\n";
		uint ctr = ++i;
		std::for_each(Base::styles->cbegin(), Base::styles->cend(), [&](const auto& s) { res += s->Out(ctr) + "\n"; });
		return res + "\n" +  intent+ "}";
	};	
};
