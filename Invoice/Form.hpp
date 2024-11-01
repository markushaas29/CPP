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
    Form(const Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>>& a, const Year& y,const std::string& p): 
		date{Date::Today().Html()},
		address(a),
		builder{HtmlBuilder<German>(std::string()+ y.ToString() +".html")}, 
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

		auto mf = MatrixFormatter(address);
		auto div1 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<GridArea,AreaNum<2>>,Style<Margin,Px<50>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div1->Add(mf.Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Caption"))));
		outs->push_back(std::move(div1));
		outs->push_back(std::move(div0));

		auto div2 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<GridArea,AreaNum<3>>,Style<Margin,Px<50>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div2->Add(mf.Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Caption"))));
		
		outs->push_back(std::move(div2));
		auto grid = HtmlElements<DivTag>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>, Style<GridTemplateAreas,DinA4>>>(), "grid-container"};
		html(grid);

		std::cout<<html<<std::endl;
	}
private:
	Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> address;
    Quantity<Sum> sum;
	std::string name;
	std::string path;
	std::unique_ptr<IHtmlElement> n;
	std::unique_ptr<IHtmlElement> date;
	HtmlBuilder<German> builder;
    friend  std::ostream& operator<<(std::ostream& out, const Form& s)   {   return out<<"Result: "<<s.address;   }
    std::ostream& display(std::ostream& out) const { return out<<(*this); }
};
