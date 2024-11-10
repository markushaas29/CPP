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
		address{std::move(a)},
		date{Date::Today().Html()},
		sum{Quantity<Sum>{321}.Html()},
		content{Entry{"Content"}.Html()},
		builder{HtmlBuilder<German>("Form.html")}, 
		path{p}{ }
    auto exec()//const HtmlBuilder<German>& f, const Year& y)  
	{
        auto html = HtmlBuilder("Invoice_.html","/home/markus/Downloads/CSV_TestFiles_2");

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto classCss = std::make_unique<StyleElement>();
		classCss->Add(std::make_unique<ClassCss<Border,Style<Padding,Px<14>>>>());
		outs->push_back(std::move(classCss));
		auto div0 = std::make_unique<HtmlElements<DivTag>>("Div0",std::make_unique<Css<Style<GridArea,AreaNum<1>>,Style<TextAlign, Right>>>());
		div0->Add(std::move(date));

		auto div1 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<GridArea,AreaNum<2>>,Style<Margin,Px<50>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div1->Add(std::move(address));
		div1->Add(std::move(content));
		div1->Add(std::move(sum));
		outs->push_back(std::move(div1));
		outs->push_back(std::move(div0));

		auto grid = HtmlElements<DivTag>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>, Style<GridTemplateAreas,DinA4>>>(), "grid-container"};
		html(grid);

		std::cout<<html<<std::endl;
	}
private:
	std::string path;
	std::unique_ptr<IHtmlElement> address;
	std::unique_ptr<IHtmlElement> date;
	std::unique_ptr<IHtmlElement> sum;
	std::unique_ptr<IHtmlElement> content;
	HtmlBuilder<German> builder;
    friend  std::ostream& operator<<(std::ostream& out, const Form& s)   {   return out<<"Result: "<<s.address;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
};
