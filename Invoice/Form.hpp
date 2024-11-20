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

		auto elements = std::vector<std::shared_ptr<IHtmlElement>>(); 

		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
		heads->push_back(std::make_unique<LinkElement>());
		std::vector<std::shared_ptr<IHtmlElement>> headElements= { std::make_unique<HtmlElements<Head>>(std::move(heads)) }; 
		elements.push_back((MatrixFormatter(Init(headElements)()).Html()));

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
//		auto classCss = std::make_unique<StyleElement>();
//		classCss->Add(std::make_unique<ClassCss<Border,Style<Padding,Px<14>>>>());
//		outs->push_back(std::move(classCss));
		std::vector<std::shared_ptr<IHtmlElement>> headlines = { empty->Html(), empty->Html(), empty->Html(), date->Clone()};
		elements.push_back(MatrixFormatter(Init(headlines)()).Html());

		std::vector<std::vector<std::shared_ptr<IHtmlElement>>> adresses = { {address->Clone(), sender->Clone()} };
		elements.push_back(MatrixFormatter(Init(std::move(adresses))()).Html());
		
		auto outR = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		outR->push_back(content->Clone());
		outs->push_back(std::make_unique<HtmlElements<Tr>>( std::move(outR) ));

		std::vector<std::shared_ptr<IHtmlElement>> contentLines = { std::move(content)};
		elements.push_back(MatrixFormatter(Init(std::move(contentLines))()).Html());
		
		std::vector<std::shared_ptr<IHtmlElement>> sumLines = { empty->Html(), empty->Html(), empty->Html(), std::move(sum)};
		elements.push_back(MatrixFormatter(Init(std::move(sumLines))()).Html());
		
		outs->push_back((MatrixFormatter(Init(std::move(elements))()).Html()));

		auto grid = HtmlElements<Table>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>>>(), "grid-container"};
		html(grid);
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
