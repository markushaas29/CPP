#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/IModel.hpp"
#include "../CSV/CssStyle.hpp"
#include "../CSV/StyleValues.hpp"
#include "../TypeCounter/TypeCounter.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

template<typename, bool> class PredicateVisitor;

#pragma once

template<typename T, typename E> struct HtmlElement; 

class IHtmlElement: public IModel, public IHtml
{
public:
	std::string Data() const  { return Out(0); };	
	const auto& Id() const  { return classId(); };	
	const auto& Name() const { return className(); };	
	virtual const std::string& Content() const  = 0;	
	virtual const std::string& Tag() const  = 0;	
	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;
private:
	virtual const std::string& classId() const  = 0;	
	virtual const std::string& className() const  = 0;	
	friend std::ostream& operator<<(std::ostream& out, const IHtmlElement& e) {	return out<<e.Out(0);}
};
////--------------------------------HtmlBase------------------------------------------------

template<typename T>
class HtmlBase: public IHtmlElement
{
protected:
	inline const static std::string tag = T::Identifier;
	inline static const std::string Identifier = T::Identifier;
	inline const static std::string end =  "</" + tag + ">";	
 	HtmlBase(std::unique_ptr<ICss> css = nullptr, const std::string& n = "", const std::string& ci = ""): begin(createBegin(css ? (*css)() : "",n,ci)), name{n}, id{ci},css{std::move(css)}, content{""} {	}
	HtmlBase(const HtmlBase& html): css(html.css ? html.css->Clone() : nullptr), begin(html.begin), content{html.Out()} { }
public:
	const std::string& Tag() const { return tag; }
	const std::string& Content() const { return content; }
	const auto Data() const { return Out(0); }
protected:
	void apply(std::unique_ptr<ICss> cs) 
	{ 
		css->Update(std::move(cs));
		begin = createBegin(css ? (*css)() : "", name, id);
	}
	std::string begin;
	std::string name;
	std::string id;
	std::unique_ptr<ICss> css;
private:
	std::string content;
	virtual const std::string& classId() const  { return id; };	
	virtual const std::string& className() const  { return name; };	
	virtual std::string out(const std::string& intent, uint i = 0) const  {	return intent + begin + showContent(intent, ++i) + intent + end; };	
	virtual std::string showContent(const std::string& intent, uint i = 0) const  = 0;	
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return Clone(); }
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return Clone(); };
	static std::string nothingIfEmpty(const std::string& s, const std::string& n)  {	return s != "" ? (" "+ n + "=\"" + s + "\"") : ""; }
	static std::string createBegin(const std::string& s, const std::string& n, const std::string& id)  {	return "<" + tag + nothingIfEmpty(id, "id") + nothingIfEmpty(n,"class") + s + ">"; };	};

template<typename T>
class HtmlElements: public HtmlBase<T>
{
	using Base = HtmlBase<T>;
public:
	inline static const std::string Identifier = "HtmlElements";
 	HtmlElements(std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> v = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(), std::unique_ptr<ICss> css = nullptr, const std::string& n = "", const std::string& id = ""): Base{std::move(css), n,id}, elements{std::move(v)} { };
 	HtmlElements(const std::string& n, const std::string& id = "", std::unique_ptr<ICss> css = nullptr, std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> v = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>()): HtmlElements{std::move(v),std::move(css),n,id}{};
	HtmlElements(const HtmlElements& html): Base{html}, elements{html.cloneElements()} { }
	void Add(std::unique_ptr<IHtmlElement> html) { elements->push_back(std::move(html)); }
	virtual std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElements>(cloneElements(), Base::css ? Base::css->Clone() : nullptr,Base::name,Base::id); };
private:
	virtual std::string showContent(const std::string& intent, uint i = 0) const  
	{	
		std::string result = "\n";
		if(elements)
			std::for_each(elements->begin(), elements->end(), [&](auto& e) { result += e->Out(i) + "\n"; });
		return result;
	};	
	std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> elements;
	std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> cloneElements() const
	{	
		auto result = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result->push_back(e->Clone()); });
		return result;
	};	
};

class StyleElement: public HtmlBase<StyleElement>
{
	using Base = HtmlBase<StyleElement>;
public:
 	StyleElement(std::unique_ptr<std::vector<std::unique_ptr<ICss>>> v = std::make_unique<std::vector<std::unique_ptr<ICss>>>(), std::unique_ptr<ICss> css = nullptr, const std::string& n = ""): Base{std::move(css), n}, elements{std::move(v)} { };
 	StyleElement( const std::string& n, std::unique_ptr<ICss> css = nullptr , std::unique_ptr<std::vector<std::unique_ptr<ICss>>> v = std::make_unique<std::vector<std::unique_ptr<ICss>>>()): StyleElement{std::move(v),std::move(css),n}{};
	StyleElement(const StyleElement& html): Base{html}, elements{html.cloneElements()} { }
	void Add(std::unique_ptr<ICss> html) { elements->push_back(std::move(html)); }
	virtual std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<StyleElement>(cloneElements()); }
	inline const static std::string Identifier = "Style";
private:
	virtual std::string showContent(const std::string& intent, uint i = 0) const  
	{	
		std::string result = "\n";
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result += e->Out(i); });
		return result;
	};	
	std::unique_ptr<std::vector<std::unique_ptr<ICss>>> elements;
	std::unique_ptr<std::vector<std::unique_ptr<ICss>>> cloneElements() const
	{	
		auto result = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result->push_back(e->Clone()); });	
		return result;
	};	
};

class LinkElement: public HtmlBase<LinkElement>
{
	using Base = HtmlBase<LinkElement>;
public:
 	LinkElement(const std::string& n = "style"): Base{nullptr, ""}, filename{n} { };
	LinkElement(const LinkElement& html): Base{html} { }
	virtual std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<LinkElement>(filename); };
	inline const static std::string Identifier = "Link";
private:
	std::string filename;
	virtual std::string out(const std::string& intent, uint i = 0) const  { return intent + "<Link rel=\"stylesheet\" href=\"" + filename + ".css\" media=\"all\"></Link>"; };	
	virtual std::string showContent(const std::string& intent, uint i = 0) const  {	return "rel=\"stylesheet\" href=\"" + filename + ".css\" media=\"all\"";	};	
};


template<typename T, typename E>
class HtmlElementBase: public HtmlBase<T>
{
	using Base = HtmlBase<T>;
protected:
 	HtmlElementBase(const E& c, std::unique_ptr<ICss> css = nullptr, const std::string& n = Identifier, const std::string& i = ClassId): Base{std::move(css),identifierIfEmpty(n), classIdIfEmpty(i)}, element{c} {	};
	HtmlElementBase(const HtmlElementBase& html): Base{html}, element{html.element} { }
public:
	inline static const std::string Identifier = std::string(std::remove_reference<E>::type::Identifier) + "HtmlElement";
	inline static const std::string ClassId = Base::Identifier + "_" + std::remove_reference<E>::type::Identifier;
	std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElement<T,E>>(element, Base::css->Clone()); };
private:
	static auto identifierIfEmpty(const std::string& n) { return n == "" ? Identifier : n; }
	static auto classIdIfEmpty(const std::string& n) { return n == "" ? ClassId : n; }
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return std::make_unique<HtmlElement<T,E>>(element, std::move(css),n,id); };
	E element;
	virtual std::string showContent(const std::string& intent, uint i = 0) const  { return "\n" + element.Out(i) + "\n";	};	
};

struct Td;

template<typename T, typename E>
struct HtmlElement: public HtmlElementBase<T,E>{ 	HtmlElement(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Black>>>(), const std::string& n="", const std::string& id=""): 
	HtmlElementBase<T,E>(c, std::move(css)) {  }; };

template<typename T>
class HtmlElement<T, IHtmlElement>: public HtmlBase<T>
{
	using Base = HtmlBase<T>;
public:
 	HtmlElement(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id=""): Base{std::move(css), n, id}, element{std::move(v)} { };
 	HtmlElement( const std::string& n, std::unique_ptr<ICss> css = nullptr, std::unique_ptr<IHtmlElement> v = nullptr): HtmlElement{std::move(v),std::move(css),n}{};
	HtmlElement(const HtmlElement& html): Base{html}, element{html.cloneElement()} { }
	HtmlElement& operator=(const HtmlElement& e) 
	{ 
		element = e.element->Clone(); 
		return *this;
	}
	virtual std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElement>(cloneElement(), nullptr, Base::name, Base::id); };
private:
	std::unique_ptr<IHtmlElement> element;
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const { return std::make_unique<HtmlElement<T,IHtmlElement>>(element->Clone(), std::move(css),n,id); };
	virtual std::string showContent(const std::string& intent, uint i = 0) const  {	return "\n" + element->Out(i) + "\n"; };	
	std::unique_ptr<IHtmlElement> cloneElement() const	{	return element ? element->Clone() : nullptr;};	
};

class Name;

template<typename T>
struct HtmlElement<T,Name>: public HtmlElementBase<T,Name>{ 	HtmlElement(const Name& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>(), 
		const std::string& n="", const std::string& id=""): HtmlElementBase<T,Name>(c, std::move(css),n,id) { }; };

class Prename;

template<typename T>
struct HtmlElement<T,Prename>: public HtmlElementBase<T,Prename> { 	HtmlElement(const Prename& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>(),
		const std::string& n="", const std::string& id=""): HtmlElementBase<T,Prename>(c, std::move(css),n,id) { }; };

class Header;

template<typename T>
struct HtmlElement<T,Header>: public HtmlElementBase<T,Header> { 	HtmlElement(const Header& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>(), 
		const std::string& n=HtmlElementBase<T,Header>::Identifier, const std::string& id=HtmlElementBase<T,Header>::ClassId): HtmlElementBase<T,Header>(c, std::move(css),n,id) { }; };

template<typename T>
struct HtmlElement<T,Quantity<Sum>>: public HtmlElementBase<T,Quantity<Sum>>
{
 	HtmlElement(const Quantity<Sum>& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Black>>>(), const std::string& n =HtmlElementBase<T,Quantity<Sum>>::Identifier, 
			const std::string& id=HtmlElementBase<T,Quantity<Sum>>::ClassId): HtmlElementBase<T,Quantity<Sum>>(c, std::move(css)) 
	{
		if(c < Quantity<Sum>{0})
			this->apply(std::make_unique<Css<Style<ColorTag,Red>>>());
		if(c > Quantity<Sum>{0})
			this->apply(std::make_unique<Css<Style<ColorTag,Green>>>());
	};
};

template<typename T, typename E>
static std::unique_ptr<IHtmlElement> Html(const E& element, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") { 	return std::make_unique<HtmlElement<T,E>>(element,std::move(css), n, id); };

template<typename T, typename E>
HtmlElement(const E&) -> HtmlElement<T,E>;

template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
