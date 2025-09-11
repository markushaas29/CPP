#include <iostream>
#include <regex>
#include <cassert>
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../HtmlElement.hpp"
#include "../NumericStyle.hpp"
#include "../Styles.hpp"
#include "../CssStyle.hpp"
#include "../Style.hpp"
#include "../../Common/Color.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"
#include "../../Matrix/MatrixHeaders.hpp"
#include "../../Matrix/Matrix.hpp"
#include "../../Matrix/MatrixDescriptor.hpp"
#include "../../Matrix/MatrixFormatter.hpp"
#include "../../Matrix/MatrixFormatter.hpp"
#include "../../Matrix/MatrixInitializer.hpp"

class StyleTest{
public:
int Run()
{     
	std::string line;
	assert(Px<14>::Id=="14px");
	assert(DynamicPx(14).Out(0)=="14px");
	auto css14 = Css<Style<Padding,Px<14>>>();
	
	auto span = Css<Style<ColSpan,StyleNumber<4>>>();
	std::cout<<"Elements: \n"<<span<<std::endl;
	assert(span()==" style=\" colspan:4;\"");

	auto nullgrid = HtmlElements<DivTag>{nullptr,nullptr, "grid-container"};
	std::istringstream f2(nullgrid.Out(0));
    std::getline(f2, line);
	assert(line=="<div class=\"grid-container\">");
	
	auto styleVec = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
	
	auto classCss = ClassCss<Style<Padding,Px<14>>>("border");
	assert(classCss()==".border {\n\t padding:14px;\n\n}");
	assert(classCss.Name()=="border");
	styleVec->push_back(classCss.Clone());
	
	auto classCss2 = ClassCss<Style<Width,Px<14>>, Style<Padding,Px<14>>, Style<Margin,Px<14>>, Style<TextAlign, Px<15>>>("border2");
	//assert(classCss2()==".border2 {\n\t width:14px;\n}");
	assert(classCss2.Name()=="border2");
	styleVec->push_back(classCss2.Clone());
	//styleVec->push_back(classCss.Clone());
	
	auto style = StyleElement{std::move(styleVec)};
	std::cout<<"StyleElement\n" << style.Data() << std::endl;
	//assert(style.Out(0)=="<Style>\n\t.border {\n\t\tpadding:14px;\n\t}\n</Style>");
	
	auto link = LinkElement{};
	assert(link.Out(0)=="<Link rel=\"stylesheet\" href=\"style.css\" media=\"all\"></Link>");
	auto linka = LinkElement{"abc"};
	assert(linka.Out(0)=="<Link rel=\"stylesheet\" href=\"abc.css\" media=\"all\"></Link>");
	assert(linka.Clone()->Out(0)=="<Link rel=\"stylesheet\" href=\"abc.css\" media=\"all\"></Link>");
    
	auto rgb = RGB<255,255,233>();
	assert(rgb()=="rgb(255, 255, 233)");
	auto rgba = RGBA<255,25,233>();
	assert(rgba()=="rgba(255, 25, 233, 1.000000)");
	auto srgb = Style<BackgroundColor,RGB<255,255,233>>();

	auto es = Px<1,2,3>::Id;
	std::cout<<"Nums: \n"<<es<<std::endl;
	es = Px<14,13>::Id;
	assert(es=="14px 13px");
	es = Px<14,13,10,12>::Id;
	assert(es=="14px 13px 10px 12px");

	es = AreaNum<112>::Id;
	assert(es=="112");
	es = AreaNum<14,13>::Id;
	assert(es=="14 13");
	es = AreaNum<14,13,10,12>::Id;
	assert(es=="14 13 10 12");

	auto hex1 = Hex<"ff6347">::Id;
	assert(hex1=="#ff6347");
	hex1 = Hex<"ff6348">::Id;
	std::cout<<"Style: \n"<<Hex<"ff6347">::Id<<std::endl;
	assert(hex1=="#ff6348");
	hex1 = Hex<"FF6348">::Id;
	assert(hex1=="#FF6348");

	assert(DynamicStyle<Padding>(std::make_unique<DynamicPx>(14)).Out(0)==" padding:14px;");
	
	auto styles = std::make_unique<std::vector<std::unique_ptr<IStyle>>>();
	styles->push_back(std::make_unique<DynamicStyle<Padding>>(std::make_unique<DynamicPx>(14)));
	styles->push_back(std::make_unique<DynamicStyle<Margin>>(std::make_unique<DynamicPx>(14)));
	auto dynCss = DynamicCss(std::move(styles));
	std::cout<<"DynStyle:"<<dynCss.Out(0)<<"1"<<std::endl;
	assert(dynCss.Out(0)==" style=\" padding:14px; margin:14px;\"\n");
	auto dynCss2 = dynCss.Clone();
	assert(dynCss2->Out(0)==" style=\" padding:14px; margin:14px;\"\n");
	

	//auto px100 = Style<Padding,Px<14>>::Get(10);

	return 0;
}
};
