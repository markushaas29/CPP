#include <memory>
#include <tuple>
#include <vector>
#include <sstream>
#include "MatrixInitializer.hpp"
#include "../CSV/IHtml.hpp"
#include "../CSV/IModel.hpp"
#include "../CSV/HtmlElement.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    
#include "../Matrix/MatrixFormatter.hpp"
#include "../CSV/NumericStyle.hpp"
#include "../Functional/Functional.hpp"    
#include "../HTMLBuilder/HTMLBuilder.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename MType>
class IResult: public IHtml, public IModel
{
public:
	using QuantityType = Q;
	using FuncType = Acc<Q>;
	using MatrixType = MType;
	using MatrixOne =  Matrix<1, MatrixDescriptor<1,std::shared_ptr<IElement>>>;
	virtual Q Value() const = 0;
	auto Names() const { return names(); };
	auto Count() const { return count(); };
	virtual const std::string& Name() const = 0;
	virtual std::ofstream& operator()(std::ofstream& s) const = 0;
	decltype(auto) operator[](const std::string& s) const { return itemsM(s); };
	decltype(auto) Elements() {	return Init(elements())(); 	};
	decltype(auto) Funcs()	{	return Init(funcs())(); };
	decltype(auto) FuncVec() { return funcs(); }
	decltype(auto) Info() { return info(); }
	virtual std::unique_ptr<IHtmlElement> printContent(int intent, std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  
	{
		auto styles = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
		styles->push_back(std::make_unique<DynamicStyle<MarginLeft>>(std::make_unique<DynamicPx>(10*intent)));

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto div = std::make_unique<HtmlElements<DivTag>>("Div0","",std::make_unique<DynamicCss>(std::move(styles)));

		auto inner = print(++intent,std::move(v),std::move(css),n,id);
		div->Add(std::move(inner));

		return div;
	}
private:
	friend 	std::ostream& operator<<(std::ostream& out, const IResult& s) {	return s.display(out);	}
	virtual MType itemsM(const std::string& s) const = 0;
	virtual std::unique_ptr<IHtmlElement> cssHtml(std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const {	return html(nullptr, nullptr,n,id);	};
	virtual std::unique_ptr<IHtmlElement> html(std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  {  return printContent(1,std::move(v),std::move(css),n,id);  };
	virtual MatrixOne names() const = 0;
	virtual size_t count() const = 0;
	virtual const std::string& info() const = 0;
	virtual std::vector<std::shared_ptr<IElement>> elements() const = 0;
	virtual std::vector<FuncType> funcs() const = 0;
	virtual std::ostream& display(std::ostream& out)	const = 0;
	virtual std::string out(const std::string& intent, uint i = 0) const  { return showContent(intent,++i); };
	virtual std::string showContent(const std::string& intent, uint i = 0) const  = 0;
	virtual std::unique_ptr<IHtmlElement> print(int i,std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  = 0;
};

template<typename Q, typename MType>
class Result: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	Result(const typename Base::FuncType&& q, const MType&& m = MType(), const std::string& n ="", const std::string& i =""): value{q()}, item(m), name{n}, infos{i}, result{q} {};
	Result(const Q&& q, const MType&& m = MType(), const std::string& n ="", const std::string& i =""): value{q}, item(m), name{n}, infos{i} {};
	virtual Q Value() const { return Q{result()}; }
	virtual MType M() { return MType(); };
	virtual const std::string& Name() const { return name; };
	virtual std::ofstream& operator()(std::ofstream& s) const 
	{ 
		auto mf = MatrixFormatter(item);  
		return HtmlBuilder()(s,mf());
	};
private:
	typename Base::QuantityType value;
	MType item;
	std::string name;
	std::string infos;
	friend 	std::ostream& operator<<(std::ostream& out, const Result& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\nValue: "<<s.value<<s.result;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual MType itemsM(const std::string& s) const 
	{ 
		//std::cout<<"OP: "<<item<<std::endl;
		return item; };
	virtual std::vector<std::shared_ptr<IElement>> elements() const	{	return std::vector<std::shared_ptr<IElement>>{ std::make_shared<Q>(result()) };	};
	virtual std::vector<typename Base::FuncType> funcs() const { return {result};};
	virtual typename Base::MatrixOne names() const 
	{
		std::vector<std::shared_ptr<IElement>> res = { std::make_shared<Header>(name) };
		return Init(res)();
	};
	virtual std::string showContent(const std::string& intent, uint i = 0) const  { return intent; };
	typename Base::FuncType result;
	virtual std::unique_ptr<IHtmlElement> print(int i,std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  {	return MatrixFormatter(item).Html(std::make_unique<Css<Style<Margin,Px<50>>>>()); };	
	virtual const std::string& info() const { return infos; };
	virtual size_t count() const { return 1; };
};

template<typename Q, typename MType>
class CompositeResult: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	CompositeResult(const Q&& q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n ="", const std::string& i =""): value{q}, items{std::move(v)},name{n}, infos{i} {};
	CompositeResult(std::shared_ptr<IElement> q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n ="", const std::string& i =""): value{*q}, items{std::move(v)},name{n}, infos{i} {};
	virtual Q Value() const { return value; }
	virtual const std::string& Name() const { return name; };
	decltype(auto) Elements() const 
	{
		std::vector<std::shared_ptr<IElement>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) { v.push_back(std::make_shared<Q>(i->Value())); });
		auto m = Init(v);
		return m(); 
	};
	virtual std::ofstream& operator()(std::ofstream& s) const 
	{	
		std::for_each(items->cbegin(), items->cend(), [&s](const auto& i) {	(*i)(s);	});
		return s;	
	}
private:
	friend 	std::ostream& operator<<(std::ostream& out, const CompositeResult& s)	
	{	
		out<<"Name: "<<s.name<<"\n";	
		std::for_each(s.items->cbegin(), s.items->cend(), [&out](const auto& i) { out<<*i<<"\n"; });
		return out<<"Value: "<<s.value;	
	}
	virtual MType itemsM(const std::string& s) const 
	{ 
		std::for_each(items->cbegin(), items->cend(), [&s](const auto& i) 
				{ 
					(*i)[s];
					//v.push_back(std::make_shared<Q>(i->Value())); 
					});
		return MType(); };
	virtual std::vector<std::shared_ptr<IElement>> elements() const
	{
		std::vector<std::shared_ptr<IElement>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) { v.push_back(std::make_shared<Q>(i->Value())); });
		return v; 
	};
	virtual std::vector<typename Base::FuncType> funcs() const
	{
		std::vector<typename Base::FuncType> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) 
				{
					auto fs = i->FuncVec();
					v.insert(v.end(), fs.begin(),fs.end());
				});
		return v; 
	};
	virtual size_t count() const { return 1; };
	virtual typename Base::MatrixOne names() const 
	{
		std::vector<std::shared_ptr<IElement>> res;
		std::for_each(items->cbegin(), items->cend(), [&res](const auto& i) {	res.push_back(std::make_shared<Header>(i->Name()));		});
		return Init(res)();
	};
	virtual std::unique_ptr<IHtmlElement> print(int in,std::unique_ptr<IHtmlElement> v = nullptr, std::unique_ptr<ICss> css = nullptr, const std::string& n="", const std::string& id="") const  
	{ 
		using DT = MType::DescriptorType;
		auto htmlPtr = std::make_unique<HtmlElements<DivTag>>("","");	
		std::for_each(items->cbegin(), items->cend(), [&](const auto& i) 
				{
					if(i->Value() != Q{0})
					{
						std::vector<std::shared_ptr<IElement>> result = { std::make_shared<Header>(i->Name()+"_"+i->Info()),i->Value().Clone() };
						auto stylesH = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
						stylesH->push_back(std::make_unique<DynamicStyle<FontSize>>(std::make_unique<DynamicPx>(60-5*in)));
						stylesH->push_back(std::make_unique<Style<FontWeight,Bold>>());
						std::unique_ptr<ICss> dynCssH = std::make_unique<DynamicCss>(std::move(stylesH));

						htmlPtr->Add(Html<Header>(Header{i->Name()},std::move(dynCssH)));
						auto styles = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
						styles->push_back(std::make_unique<DynamicStyle<MarginLeft>>(std::make_unique<DynamicPx>(5)));
						styles->push_back(std::make_unique<DynamicStyle<Padding>>(std::make_unique<DynamicPx>(20)));
						styles->push_back(std::make_unique<DynamicStyle<FontSize>>(std::make_unique<DynamicPx>(40-5*in)));
						styles->push_back(std::make_unique<Style<FontWeight,Bold>>());
						std::unique_ptr<ICss> dynCss = std::make_unique<DynamicCss>(std::move(styles));

						auto dynCss2 = dynCss->Clone();
						htmlPtr->Add(MatrixFormatter(Init(result)()).Html(std::move(dynCss)));
				
						std::vector<std::vector<std::shared_ptr<IElement>>> resultFuncs ;
						auto funcs = i->FuncVec();
						std::for_each(funcs.cbegin(),funcs.cend(), [&](const auto& f) 
								{
									if(f.Size()!=0)
									{
										std::vector<std::shared_ptr<IElement>> resultf ;
										std::ostringstream os;
										os<<f;
										resultf.push_back(std::make_shared<Header>(i->Name()));
										resultf.push_back(std::make_shared<Entry>(os.str()));
										resultf.push_back(std::make_shared<Header>("="));
										resultf.push_back(std::make_shared<Q>(f()));
										resultFuncs.push_back(resultf);
										std::vector<std::shared_ptr<IElement>> result = { std::make_shared<Header>(i->Name()+"_"+i->Info()),i->Value().Clone() };
									}
								});
					
						htmlPtr->Add(MatrixFormatter((Init(resultFuncs)())).Html(std::move(dynCss2)));
						htmlPtr->Add(i->printContent(in+2));
					}
					});

		return htmlPtr;
	};
	virtual std::string showContent(const std::string& intent, uint i = 0) const { return intent; };
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual const std::string& info() const { return infos; };
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
	std::string infos;
};
