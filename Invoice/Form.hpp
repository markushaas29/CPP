#include <memory>
#include <tuple>
#include <vector>
#include <sstream>
#include <chrono>
#include "InvoiceCalculatorBase.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Styles.hpp"
#include "../CSV/Style.hpp"
#include "../CSV/StyleValues.hpp"
#include "../CSV/NumericStyle.hpp"
#include "../Functional/Functional.hpp"

#pragma once

template<typename T>
class Form
{
    using Stage = T;
public:
    Form(std::unique_ptr<IHtmlElement> a, const std::string& p, const std::string& n = ""): 
		path{p},
		filename{n},
		phone{createPhone()},
		sender{createSender()},
		bank{createBank()},
		ending{createEnding()},
		address{std::move(a)},
		date{Date::Today().Html()},
		content{Entry{"Content"}.Html()},
		builder{HtmlBuilder<German>("Form.html")} { }
    auto exec()//const HtmlBuilder<German>& f, const Year& y)  
	{
        auto html = HtmlBuilder(filename+".html","/home/markus/Dokumente/cpp/CSV_Files");

		auto elements = std::vector<std::shared_ptr<IHtmlElement>>(); 

		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
		heads->push_back(std::make_unique<LinkElement>());
		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();

		auto styleVec = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
		styleVec->push_back(std::make_unique<ClassCss<Style<Width,Px<1600>>, Style<Margin,Px<50>>, Style<FontSize, Px<25>>>>("addressLine"));
		styleVec->push_back(std::make_unique<ClassCss<Style<Width,Px<1800>>>>("mainBody"));
		
		auto body = std::make_unique<HtmlElements<Body>>("mainBody","mainBody",std::make_unique<Css<Style<Margin,Px<75>>>>());
		heads->push_back(std::make_unique<StyleElement>(std::move(styleVec)));
		html.Add(std::make_unique<HtmlElements<Head>>(std::move(heads))); 

		auto r = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>, Style<FloatTag,Right>>>());
		r->Add(std::make_unique<HtmlElement<DivTag, IHtmlElement>>(phone->Clone(),std::make_unique<Css<Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<Margin,Px<25>>, Style<FloatTag,Right>>>(),  "phone"));
		r->Add(Html<DivTag>(Date::Today(),std::make_unique<Css<Style<FontWeight,Bold>, Style<Margin,Px<25>>, Style<PaddingTop,Px<250>>,Style<FontSize,Px<25>>>>(),"Date", "DateId"));

        body->Add(std::move(r));
		body->Add(std::move(sender));
        body->Add(std::make_unique<HtmlElement<DivTag, IHtmlElement>>(address->Clone(),std::make_unique<Css<Style<FontSize,Px<25>>, Style<PaddingTop,Px<20>>, Style<MarginLeft,Px<75>>>>(), "adress"));
		
		content = Html<DivTag>(Header{"Content"},std::make_unique<Css<Style<FontWeight,Bold>, Style<Margin,Px<100>>, Style<FontSize,Px<25>>>>(),"Header", "Header");
		outs->push_back(std::make_unique<HtmlElements<Tr>>( row(content->Clone()) ));

		body->Add(std::make_unique<HtmlElement<DivTag,IHtmlElement>>(table(std::move(outs))));
		
		body->Add(std::move(ending));
		body->Add(std::move(bank));
		html.Add(std::move(body));
	}
private:
	inline static std::shared_ptr<IElement> empty = std::make_shared<Empty>("");
	std::string path;
	std::string filename;
	std::unique_ptr<IHtmlElement> phone;
	std::unique_ptr<IHtmlElement> sender;
	std::unique_ptr<IHtmlElement> bank;
	std::unique_ptr<IHtmlElement> ending;
	std::unique_ptr<IHtmlElement> address;
	std::unique_ptr<IHtmlElement> date;
	std::unique_ptr<IHtmlElement> content;
	HtmlBuilder<German> builder;
    friend  std::ostream& operator<<(std::ostream& out, const Form& s)   {   return out<<"Result: "<<s.address;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
	static auto createEnding()
	{
		std::vector<std::vector<std::shared_ptr<IElement>>> a ={
			{std::make_shared<Entry>("Greets"),std::make_shared<Empty>("")},
			{std::make_shared<Prename>("Markus"), std::make_shared<Name>("Haas")}
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<25>>, Style<MarginLeft,Px<75>>>>());
	}
	static auto createBank()
	{
		std::vector<std::shared_ptr<IElement>> a ={
			 std::make_shared<Entry>("IBAN"), std::make_shared<Entry>("DE83200411330694752700"),
			 std::make_shared<Entry>("BIC"), std::make_shared<Entry>("COBADEHD001"),
			 std::make_shared<Name>("comdirect"), std::make_shared<Empty>(""),
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>, Style<MarginLeft,Px<75>>>>());
	}
	static auto createSender()
	{
		std::vector<std::shared_ptr<IElement>> send = {
			std::make_shared<Prename>("Markus"), std::make_shared<Name>("Haas"),
			std::make_shared<Street>("Ruchenstrasse"), std::make_shared<StreetNumber>("14"),
			std::make_shared<Postcode>("76706"), std::make_shared<Town>("Dettenheim")
		};

		return MatrixFormatter(Init(send)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>, Style<MarginLeft,Px<75>>>>());
	}
	static auto createPhone()
	{
		std::vector<std::vector<std::shared_ptr<IElement>>> a ={
			{ std::make_shared<Entry>("Telephone"), std::make_shared<Entry>("07255/725393")},
			{ std::make_shared<Entry>("Handy"), std::make_shared<Entry>("0176/84733560")},
			{ std::make_shared<Entry>("Email"), std::make_shared<Entry>("markushaas1@gmx.de")},
		};

		return MatrixFormatter(Init(a)()).Lines();
	}
	
	static auto row(std::unique_ptr<IHtmlElement> html)
	{
		auto vec = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		vec->push_back(html->Clone());
		return vec;
	}
	
	static auto table(std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> html)
	{
		return std::make_unique<HtmlElements<Table>>(std::move(html));
	}
};
