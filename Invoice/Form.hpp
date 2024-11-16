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
    Form(std::unique_ptr<IHtmlElement> a, const std::string& p): 
		path{p},
		sender{createSender()},
		address{std::move(a)},
		date{Date::Today().Html()},
		sum{Quantity<Sum>{321}.Html()},
		content{Entry{"Content"}.Html()},
		builder{HtmlBuilder<German>("Form.html")} { }
    auto exec()//const HtmlBuilder<German>& f, const Year& y)  
	{
        auto html = HtmlBuilder("Invoice_.html","/home/markus/Dokumente/cpp/CSV_Files");

		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
		heads->push_back(std::make_unique<LinkElement>());
		auto head = HtmlElements<Head>{ std::move(heads) };
		html(head);

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto classCss = std::make_unique<StyleElement>();
		classCss->Add(std::make_unique<ClassCss<Border,Style<Padding,Px<14>>>>());
		outs->push_back(std::move(classCss));
		auto div0 = std::make_unique<HtmlElements<DivTag>>("Div0",std::make_unique<Css<Style<TextAlign, Right>>>());
		std::vector<std::shared_ptr<IHtmlElement>> headlines = { empty->Html(), empty->Html(), empty->Html(), date->Clone()};
		div0->Add(MatrixFormatter(Init(headlines)()).Html());

		auto div1 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<Margin,Px<50>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		std::vector<std::vector<std::shared_ptr<IHtmlElement>>> adresses = { {address->Clone(), sender->Clone()} };
		
		div1->Add(address->Clone());
		div1->Add(MatrixFormatter(Init(std::move(adresses))()).Html());
		div1->Add(std::move(content));
		div1->Add(std::move(sum));
		outs->push_back(std::move(div1));
		outs->push_back(std::move(div0));

		auto grid = HtmlElements<DivTag>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>>>(), "grid-container"};
		html(grid);

		std::cout<<html<<std::endl;
	}
private:
	inline static std::shared_ptr<IElement> empty = std::make_shared<Empty>("");
	std::string path;
	std::unique_ptr<IHtmlElement> sender;
	std::unique_ptr<IHtmlElement> address;
	std::unique_ptr<IHtmlElement> date;
	std::unique_ptr<IHtmlElement> sum;
	std::unique_ptr<IHtmlElement> content;
	HtmlBuilder<German> builder;
    friend  std::ostream& operator<<(std::ostream& out, const Form& s)   {   return out<<"Result: "<<s.address;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
	static auto createSender()
	{
		std::vector<std::vector<std::shared_ptr<IElement>>> a ={
			{ std::make_shared<Prename>("Markus"), std::make_shared<Name>("Haas")},
			{ std::make_shared<Street>("Ruchenstrasse"), std::make_shared<StreetNumber>("14")},
			{ std::make_shared<Postcode>("76706"), std::make_shared<Town>("Dettenheim")},
		};

		return MatrixFormatter(Init(a)()).Html();
	}
};
