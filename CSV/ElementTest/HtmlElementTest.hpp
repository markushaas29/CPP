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

class HtmlElementTest{
public:
int Run()
{     
	using M2DS = MatrixDescriptor<2,double>;
	using M2D = Matrix<2,M2DS>;
	std::string line;

	auto html = HtmlElement<Td, Entry>(Entry("TEST"));
	assert(html.Data()=="<td id=\"td_Entry\" class=\"EntryHtmlElement\" style=\" color:black;\">\n\tTEST\n</td>");
	assert(html.Id()=="td_Entry");
	assert(html.Name()=="EntryHtmlElement");
	auto htmlN = HtmlElement<Td, Name>(Name("TEST"));
	assert(htmlN.Data()=="<td id=\"td_Name\" class=\"NameHtmlElement\" style=\" color:blue;\">\n\tTEST\n</td>");

	auto htmlQP = HtmlElement<Td, Quantity<Sum>>(Quantity<Sum>{2});
	auto htmlQP2 = htmlQP.Clone();
	assert(htmlQP.Data()=="<td id=\"td_Sum\" class=\"SumHtmlElement\" style=\" color:green;\">\n\t2.00€\n</td>");
	std::cout<<"P OUT: \n"<<htmlQP2->Data()<<std::endl;
	assert(htmlQP2->Data()=="<td id=\"td_Sum\" class=\"SumHtmlElement\" style=\" color:green;\">\n\t2.00€\n</td>");
	auto htmlQN = HtmlElement<Td, Quantity<Sum>>(Quantity<Sum>{-2});
	assert(htmlQN.Data()=="<td id=\"td_Sum\" class=\"SumHtmlElement\" style=\" color:red;\">\n\t-2.00€\n</td>");

	auto nhtml = HtmlElement<Td,HtmlElement<Td, Name>>(htmlN);
	std::cout<<"P OUT: \n"<<nhtml.Data()<<std::endl;
	//assert(nhtml.Data()=="<td id=\"td_NameHtmlElement\" class=\"td_HtmlElement\" style=\" color:black;\">\n\t<td id=\"td_NameHtmlElement\" class=\"td_HtmlElement\" style=\" color:blue;\">\n\t\tTEST\n\t</td>\n</td>");

	M2D m33 =
    {
           {1., 2., 3.},
           {4., 5., 6.},
           {7., 8., 9.}
     };

	auto mf1 = MatrixFormatter(m33);
	
	auto cap = HtmlElement<Caption, Header>(Header("H"));
	//assert(cap.Out()=="");
	auto htmlMF = mf1.Html(cap.Clone());

	auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
	outs->push_back(htmlQN.Clone());
	outs->push_back(htmlQP.Clone());
	auto comp = HtmlElements<DivTag>{std::move(outs)};
	auto htmls = HtmlElement<Td,HtmlElements<DivTag>>(comp);
	
	auto outs2 = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
	outs2->push_back(htmlQN.Clone());
	outs2->push_back(htmlQP.Clone());
	auto grid = HtmlElements<DivTag>{std::move(outs2),std::make_unique<Css<Style<Display,Grid>>>(), "grid-container"};
	std::cout<<"\n\nG: \n"<<grid.Out(0)<<std::endl;
	std::istringstream f(grid.Out(0));
    std::getline(f, line);

	auto nullgrid = HtmlElements<DivTag>{std::move(outs2),nullptr, "grid-container"};
	std::istringstream f2(nullgrid.Out(0));
    std::getline(f2, line);
	assert(line=="<div class=\"grid-container\">");
	
	auto styleVec = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
	
	auto style = StyleElement{std::move(styleVec)};
	std::cout<<"StyleElement\n" << style.Data() << std::endl;
	//assert(style.Out(0)=="<Style>\n\t.border {\n\t\tpadding:14px;\n\t}\n</Style>");
	
	auto link = LinkElement{};
	assert(link.Out(0)=="<Link rel=\"stylesheet\" href=\"style.css\" media=\"all\"></Link>");
	auto linka = LinkElement{"abc"};
	assert(linka.Out(0)=="<Link rel=\"stylesheet\" href=\"abc.css\" media=\"all\"></Link>");
	assert(linka.Clone()->Out(0)=="<Link rel=\"stylesheet\" href=\"abc.css\" media=\"all\"></Link>");
	
	auto namePtr = std::make_unique<HtmlElement<Td,Name>>(Name("ABC"));
	auto ihtml = HtmlElement<Td, IHtmlElement>(std::move(namePtr));
	ihtml = HtmlElement<Td, IHtmlElement>(mf1.Html());
	auto pSpan = std::make_unique<Css<Style<ColSpan,StyleNumber<4>>>>();
	auto modHtml = ihtml.Html(std::move(pSpan));
	std::istringstream f3(modHtml->Out(0));
    std::getline(f3, line);
	assert(line=="<td style=\" colspan:4;\">");

	auto date = Date::Today().Clone();
	std::vector<std::vector<std::shared_ptr<IElement>>> headEls = {{ date->Clone()},{ date->Clone()}};
	auto m = Init(headEls)();
	auto mh = MatrixFormatter(m).Html();
	
	std::stringstream ssC(mh->Out(0));
	std::string sC;
    int cnt=0;
    while(ssC>>sC)
		if (std::string::npos != sC.find("table"))
          cnt++;

	assert(cnt==2);
	
	auto empty = std::make_shared<Empty>("");
	std::vector<std::shared_ptr<IHtmlElement>> headlines = { empty->Html(), empty->Html(), empty->Html(), date->Html()->Clone()};

	auto m2 = Init(headlines)();
	mh = MatrixFormatter(m2).Html();
	
	std::stringstream ssC2(mh->Out(0));
    cnt=0;
	sC = "";
    while(ssC2>>sC)
		if (std::string::npos != sC.find("table"))
          cnt++;

	assert(cnt==2);

	auto htmlEabc = HtmlElement<Td, Entry>(Entry("TEST"));
	assert(htmlEabc.Data()=="<td id=\"td_Entry\" class=\"EntryHtmlElement\" style=\" color:black;\">\n\tTEST\n</td>");
	assert(htmlEabc.Id()=="td_Entry");
	assert(htmlEabc.Name()=="EntryHtmlElement");
	return 0;
}
};
