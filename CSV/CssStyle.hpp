#include "Style.hpp"
#include <vector>

template<typename, bool> class PredicateVisitor;

#pragma once

class ICss//: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
//	virtual bool operator==(const ICss& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const ICss& e) const noexcept = 0;
	std::string operator()() const  { return data(); };	
//	virtual std::unique_ptr<ICss> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(Data()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const ICss& e) {	return out<<e.data();}
	virtual std::string data() const  = 0;	
////	virtual constexpr bool operator==(const ICss& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const ICss& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return data.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const Css& e) const{ return Data() == e.Data(); };
////	constexpr std::strong_ordering operator<=>( const Css& e) const noexcept { return Data() <=> e.Data(); }
};
////--------------------------------Css------------------------------------------------

template<typename... T>
class Css: public ICss
{
	 using Tup = std::tuple<T...>;
public:
 	Css(const std::string& c = ""): styles{create()} { };
private:
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
};

