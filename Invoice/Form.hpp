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

struct Comdirect
{
	static auto Get()
	{
		std::vector<std::shared_ptr<IElement>> a ={
			 std::make_shared<Entry>("IBAN"), std::make_shared<Entry>("DE83200411330694752700"),
			 std::make_shared<Entry>("BIC"), std::make_shared<Entry>("COBADEHD001"),
			 std::make_shared<Name>("comdirect"), std::make_shared<Empty>(""),
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<20>>>>());
	}
};

struct Raiffeisenbank
{
	static auto Get()
	{
		std::vector<std::shared_ptr<IElement>> a ={
			 std::make_shared<Entry>("IBAN"), std::make_shared<Entry>("DE19660623660009232702"),
			 std::make_shared<Entry>("BIC"), std::make_shared<Entry>("GENODE61DET"),
			 std::make_shared<Name>("RAIFFEISENBANK HARDT-BRUHRAIN"), std::make_shared<Empty>(""),
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<20>>>>());
	}
};

struct Signature
{
	static auto Ge()
	{
		std::vector<std::shared_ptr<IElement>> a ={
			 std::make_shared<Entry>("Dettenheim, "), std::make_shared<Date>(Date::Today())
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<20>>>>());
	}
	static auto Get()
	{
		auto end = std::make_unique<HtmlElements<DivTag>>("Ending","",std::make_unique<Css<Style<Margin,Px<5>>, Style<PaddingTop,Px<150>>, Style<Padding,Px<100>>>>());
		auto r = std::make_unique<HtmlElements<DivTag>>();
		r->Add(Html<DivTag>(Entry{"Dettenheim, "},std::make_unique<Css<Style<FontSize,Px<25>>, Style<FloatTag,Left>>>()));
		r->Add(Html<DivTag>(Date::Today(),std::make_unique<Css<Style<FontSize,Px<25>>, Style<FloatTag,Left>>>()));
		end->Add(std::move(r));
		
		auto r2 = std::make_unique<HtmlElements<DivTag>>("Ending2","",std::make_unique<Css<Style<PaddingTop,Px<150>>>>());
		r2->Add(Html<DivTag>(Entry{"...................................."},std::make_unique<Css<Style<FontSize,Px<25>>, Style<FloatTag,Left>>>()));
		end->Add(std::move(r2));
		
		auto r3 = std::make_unique<HtmlElements<DivTag>>("Ending2","",std::make_unique<Css<Style<PaddingTop,Px<40>>>>());
		r3->Add(Html<DivTag>(Name{"Markus Haas"},std::make_unique<Css<Style<FontSize,Px<25>>, Style<FloatTag,Left>>>()));
		end->Add(std::move(r3));

		return std::move(end);
	}
};

template<typename A>
class Form
{
    using Account = A;
public:
    auto exec()//const HtmlBuilder<German>& f, const Year& y)  
	{
        auto html = HtmlBuilder(filename+".html","/home/markus/Dokumente/cpp/CSV_Files");

		auto elements = std::vector<std::shared_ptr<IHtmlElement>>(); 

		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
		heads->push_back(std::make_unique<LinkElement>());
		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();

		auto styleVec = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
		styleVec->push_back(std::make_unique<ClassCss<Style<Width,Px<1600>>, Style<Margin,Px<5>>, Style<FontSize, Px<25>>>>("addressLine"));
		styleVec->push_back(std::make_unique<ClassCss<Style<Width,Px<1100>>>>("mainBody"));
		
		auto body = std::make_unique<HtmlElements<Body>>("mainBody","mainBody",std::make_unique<Css<Style<Margin,Px<5>>, Style<Padding,Px<75>>>>());
		heads->push_back(std::make_unique<StyleElement>(std::move(styleVec)));
		html.Add(std::make_unique<HtmlElements<Head>>(std::move(heads))); 

		auto r = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>, Style<Margin,Px<100>>, Style<FloatTag,Right>>>());
		r->Add(std::make_unique<HtmlElement<DivTag, IHtmlElement>>(contact->Clone(),std::make_unique<Css<Style<FontSize,Px<25>>, Style<PaddingTop,Px<120>>, Style<Margin,Px<2>>, Style<FloatTag,Right>>>(),  "contact"));
		r->Add(Html<DivTag>(Date::Today(),std::make_unique<Css<Style<FontWeight,Bold>, Style<Margin,Px<2>>, Style<PaddingTop,Px<300>>,Style<FontSize,Px<25>>>>(),"Date", "DateId"));

        body->Add(std::move(r));
		body->Add(std::move(sender));
        body->Add(std::make_unique<HtmlElement<DivTag, IHtmlElement>>(address->Clone(),std::make_unique<Css<Style<FontSize,Px<25>>, Style<PaddingTop,Px<20>>>>(), "adress"));
		
		if(content)
			body->Add(std::move(content));
		
		body->Add(std::move(ending));
		body->Add(std::move(bank));
		html.Add(std::move(body));
	}
protected:
    Form(std::unique_ptr<IHtmlElement> a,std::unique_ptr<IHtmlElement> c, const std::string& p, const std::string& n = ""): 
		path{p},
		filename{n},
		contact{createContact()},
		sender{createSender()},
		bank{Account::Get()},
		ending{Signature::Get()},
		address{std::move(a)},
		date{Date::Today().Html()},
		content{std::move(c)},
		builder{HtmlBuilder<German>("Form.html")} { }
private:
	inline static std::shared_ptr<IElement> empty = std::make_shared<Empty>("");
	std::string path;
	std::string filename;
	std::unique_ptr<IHtmlElement> contact;
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

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<25>>>>());
	}
	static auto createEnding2()
	{
		auto r1 = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>, Style<Padding,Px<10>>>>());
		r1->Add(Html<DivTag>(Text("Unterschrift Markus Haas"),std::make_unique<Css<Style<FontWeight,Bold>, Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<FloatTag,Left>>>(),  "contact"));
		//r1->Add(Html<DivTag>(Entry("signature landlord"),std::make_unique<Css<Style<FontWeight,Bold>, Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<FloatTag,Left>>>(),  "contact"));
		//r1->Add(Html<DivTag>(Entry("signature tenant"),std::make_unique<Css<Style<FontWeight,Bold>, Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<FloatTag, Right>>>(),"Date", "DateId"));
		
		auto r2 = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>, Style<Padding,Px<10>>>>());
		r2->Add(Html<DivTag>(Entry("..........."),std::make_unique<Css<Style<FontWeight,Bold>, Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<FloatTag,Left>>>(),  "contact"));
		r2->Add(Html<DivTag>(Entry("..........."),std::make_unique<Css<Style<FontWeight,Bold>, Style<FontSize,Px<25>>, Style<PaddingTop,Px<100>>, Style<FloatTag, Right>>>(),"Date", "DateId"));
		
		auto r = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>, Style<Padding,Px<100>>, Style<Margin,Px<10>>>>());
		r->Add(std::move(r1));
//		r->Add(std::move(r2));

		return r;
	}
	static auto createSender()
	{
		std::vector<std::shared_ptr<IElement>> send = {
			std::make_shared<Prename>("Markus"), std::make_shared<Name>("Haas"),
			std::make_shared<Street>("Ruchenstrasse"), std::make_shared<StreetNumber>("14"),
			std::make_shared<Postcode>("76706"), std::make_shared<Town>("Dettenheim")
		};

		return MatrixFormatter(Init(send)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<FontSize,Px<15>>>>());
	}
	static auto createContact()
	{
		std::vector<std::vector<std::shared_ptr<IElement>>> a ={
			{ std::make_shared<Entry>("Telephone"), std::make_shared<Entry>("07255/3971244")},
			{ std::make_shared<Entry>("Handy"), std::make_shared<Entry>("0176/84733560")},
			{ std::make_shared<Entry>("Email"), std::make_shared<Entry>("markushaas1@gmx.de")},
		};

		return MatrixFormatter(Init(a)()).Lines(std::make_unique<Css<Style<FontSize,Px<20>>>>());
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

class Contract: public Form<Comdirect> 
{
public:
	Contract(std::unique_ptr<IHtmlElement> a,std::unique_ptr<IHtmlElement> c, const std::string& p, const std::string& n = ""): Form<Comdirect>(std::move(a), std::move(c), p, n){}
};
class ExtraCostInvoice: public Form<Raiffeisenbank> 
{
public:
	ExtraCostInvoice(std::unique_ptr<IHtmlElement> a,std::unique_ptr<IHtmlElement> c, const std::string& p, const std::string& n = ""): Form<Raiffeisenbank>(std::move(a), std::move(c), p, n){}
};
