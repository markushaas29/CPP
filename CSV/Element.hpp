#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Is/Is.hpp"
#include "../String/String_.hpp"
#include "../String/Literal.hpp"
#include "../Visitor/Visitor.hpp"
#include "../TypeCounter/TypeCounter.hpp"
#include "IModel.hpp"
#include "IHtml.hpp"
#include "Translator.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

template<typename, bool> class PredicateVisitor;
struct Td;

template<typename,typename> class HtmlElement;

#pragma once
class IElement: public BaseVisitable<void>, public BoolVisitable<bool>, public IModel, public IHtml
{
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	//template<typename U> using IsT = typename Is<typename U,LiteralType>;
public:
	virtual void Accept(BaseVisitor& visitor) = 0;
	virtual bool Is(BaseVisitor& visitor) = 0;
	const std::string& Data() const  { return data(); };	
	virtual std::unique_ptr<IElement> Clone() const  = 0;	
	virtual std::unique_ptr<IElement> Create(const std::string& s) const  = 0;	
	virtual std::unique_ptr<IModel> Model() const { return Clone(); };
	virtual bool operator==(const IElement& e) const { return Data() == e.Data(); };
	template<typename T>
    T To() const { return ::To<T>(Data()); }
	template<typename O>
	decltype(auto) As() 
    {
        if(auto p = dynamic_cast<O*>(this))
            return *p;
     //   IsT<Throwing>(Format("INVALID AS CAST!"))(false);
        throw;
    }

private:
	virtual const std::string& data() const  = 0;	
	virtual const std::string out() const  = 0;	
	virtual std::string out(const std::string& intent, uint i = 0) const { return intent + out(); };
	friend std::ostream& operator<<(std::ostream& out, const IElement& e) {	return out<<e.Data();}
};
//--------------------------------Element------------------------------------------------

template<typename D>
class Element: public IElement, public TypeCounter<D>
{
	using Derived = D;
public:
	inline static const std::string Identifier = D::Identifier;
 	Element(const std::string& s): value{Translator::Instance()(Derived::check(s))}, size{s.size()} { };

	virtual std::unique_ptr<IElement> Clone() const  { return std::make_unique<Derived>(value); };	
	virtual std::unique_ptr<IElement> Create(const std::string& s) const  { return std::make_unique<Derived>(s); };	
	static std::unique_ptr<IElement> Make(const std::string& s) { return std::make_unique<Derived>(s);	}
	static auto Extract(const std::string& s) 	
	{ 	
		std::stringstream ss(s);
		std::vector<std::string> v;
		
	    while (ss.good()) {
			std::string substr;
			std::getline(ss, substr, ':');
	        v.push_back(String_::Trim(substr));
		}
		return Make(v[1]);	}
	explicit operator std::string() const  {	return value; };	
	constexpr decltype(auto) Size() { return size; }

	template<typename T>
	std::unique_ptr<IHtmlElement> HtmlTag(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  { return std::make_unique<HtmlElement<T,Derived>>(Derived(value));	};	
	virtual void Accept(BaseVisitor& visitor) {	return AcceptImpl<D>(*dynamic_cast<D*>(this), visitor); }
	virtual void Accept(BaseVisitor& visitor) const 
	{
		auto c2 = dynamic_cast<const D*>(this);
		auto c = const_cast<D*>(c2);
		return AcceptImpl<D>(*c, visitor); 
	}
	virtual bool Is(BaseVisitor& visitor) { return AcceptPredicate<D>(*dynamic_cast<D*>(this), visitor); };
	constexpr bool operator==(const IElement& e) const{ return Data() == e.Data(); };
	constexpr bool operator==(const std::string& e) const noexcept { return e == data(); };
	constexpr std::strong_ordering operator<=>(const IElement& e) const noexcept { return Data() <=> e.Data(); }
private:
	std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  { return HtmlTag<Td>();	};	
	std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const {	return html(nullptr, css ? std::move(css) : nullptr,n,id);	};
	const std::string& data() const  {	return value;	};	
	const std::string out() const  
	{	
  		if constexpr (std::is_same_v<Derived, Quantity<Sum>> || std::is_same_v<Derived, Quantity<Area>>)
  		{
      		std::ostringstream oss;
      		oss << *(static_cast<const Derived*>(this));
      		return oss.str();         
  		}
  		if constexpr (std::is_same_v<Derived, Quantity<Scalar,Pure,double>>)
  		{
      		std::ostringstream oss;
      		auto d =*(static_cast<const Derived*>(this));
      		oss << *(static_cast<const Derived*>(this));
      		return oss.str();         
  		}
		else
			return value; 
	};	
	std::string value;
	std::size_t size;
};


template <typename T>
concept ElementConcept = requires(T val) {	val.Data(); };
