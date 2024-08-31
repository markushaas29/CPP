#include <iostream>
#include <regex>
#include <cassert>
#include "ElementRegexTest.hpp"
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../HtmlElement.hpp"
#include "../NumericStyle.hpp"
#include "../../Decorator/ElementDecorator.hpp"
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

class ElementTest{
public:
int Run()
{     
	using M2DS = MatrixDescriptor<2,double>;
	using M2D = Matrix<2,M2DS>;
    std::cout<<"START"<<std::endl;
	
	auto s = std::string("123");
	
	auto s1 = std::string("DE123DE");
	auto s2 = std::string("DE05100110012620778704");
	auto se = std::string("701033135722 Strom Abschlagsforderung DS-Info: enbw.com/datenschutz EREF: B23505497376 701033135722 MREF: V5500000054391824 CRED: DE6900000000084184 IBAN: DE56600501017402051588 BIC: SOLADEST600;");
	auto line = std::string("Kontokorrent-/Girokonto;DE19660623660009232702;GENODE61DET;RAIFFEISENBANK HARDT-BRUHRAIN;06.11.2023;06.11.2023;EnBW Energie Bad-Wuertt AG;DE56600501017402051588;SOLADEST600;Basislastschrift;701033135722 Strom Abschlagsforderung DS-Info: enbw.com/datenschutz EREF: B23505497376 701033135722 MREF: V5500000054391824 CRED: DE6900000000084184 IBAN: DE56600501017402051588 BIC: SOLADEST600;-13,00;EUR;285,66;;Sonstiges;;DE6900000000084184;V5500000054391824");
	auto i = IBAN(s1.c_str());
	auto i2 = IBAN(s2.c_str());
	auto i3 = IBAN("");
    std::cout<<i.Data()<<std::endl;
	assert(!i.Valid());
	assert(!i3.Valid());
	assert(i2.Valid());

	std::vector<std::unique_ptr<IElement>> v;
	v.push_back(std::move(std::make_unique<IBAN>(s2.c_str())));
	v.push_back(std::move(std::make_unique<Name>(s1.c_str())));
	v.push_back(std::move(std::make_unique<Value<int>>(23)));
	v.push_back(std::move(std::make_unique<Value<double>>(23.456)));
	v.push_back(std::move(std::make_unique<ID<int>>(23)));
	v.push_back(std::move(std::make_unique<ID<std::string>>(s)));
	v.push_back(std::move(std::make_unique<Date>(29,9,1986)));

	for(auto& p : v)
		std::cout<<"P: "<<p->Data()<<" "<<*(p->Clone())<<std::endl;

	Factory<IElement> fmt;
    fmt.Register("Q",[](const std::string& s) { return std::make_unique<Quantity<Sum>>(s); });
    fmt.Register("IBAN",[](const std::string& s) { return std::make_unique<IBAN>(s); });
    
    auto qp = fmt("Q","100");   

    assert("100.kg"==(std::string)Quantity<Mass>(100));
    assert("100.12€"==(std::string)Quantity<Sum>(100.12));
	//assert(*qp==Quantity<Sum>(100));

	ElementRegexTest er;
	assert(er.Run());

	auto useE = Entry(se);
	auto useA = Entry("Abschlagsforderung");
	assert(useE==useA);
	useA = Entry("Abschagsforderung");
	assert(useE!=useA);
	useA = Entry("DE6900000000084184");
	assert(useE==useA);

	auto html = HtmlElement<Td, Entry>(Entry("TEST"));
	std::cout<<"P OUT: \n"<<html.Data()<<std::endl;
	assert(html.Data()=="<td style=\" color:black;\">\n\tTEST\n</td>");
	auto htmlN = HtmlElement<Td, Name>(Name("TEST"));
	assert(htmlN.Data()=="<td style=\" color:blue;\">\n\tTEST\n</td>");

	auto htmlQP = HtmlElement<Td, Quantity<Sum>>(Quantity<Sum>{2});
	auto htmlQP2 = htmlQP.Clone();
	assert(htmlQP.Data()=="<td style=\" color:green;\">\n\t2.00€\n</td>");
	assert(htmlQP2->Data()=="<td style=\" color:green;\">\n\t2.00€\n</td>");
	auto htmlQN = HtmlElement<Td, Quantity<Sum>>(Quantity<Sum>{-2});
	assert(htmlQN.Data()=="<td style=\" color:red;\">\n\t-2.00€\n</td>");

	auto nhtml = HtmlElement<Td,HtmlElement<Td, Name>>(htmlN);
	std::cout<<"P2: \n"<<nhtml.Data()<<std::endl;
	assert(nhtml.Data()=="<td style=\" color:black;\">\n\t<td style=\" color:blue;\">\n\t\tTEST\n\t</td>\n</td>");

	M2D m33 =
    {
           {1., 2., 3.},
           {4., 5., 6.},
           {7., 8., 9.}
     };

	auto mf1 = MatrixFormatter(m33);
	
	auto htmlMF = mf1.Html();

	auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
	outs->push_back(htmlQN.Clone());
	outs->push_back(htmlQP.Clone());
	auto comp = HtmlElements<DivTag>{std::move(outs)};
	std::cout<<"\n\nElements: \n"<<comp.Out(0)<<std::endl;
	auto htmls = HtmlElement<Td,HtmlElements<DivTag>>(comp);
	
	auto outs2 = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
	outs2->push_back(htmlQN.Clone());
	outs2->push_back(htmlQP.Clone());
	auto grid = HtmlElements<DivTag>{std::move(outs2),std::make_unique<Css<Style<Display,Grid>>>(), "grid-container"};
	std::cout<<"\n\nG: \n"<<grid.Out(0)<<std::endl;
	std::istringstream f(grid.Out(0));
    std::getline(f, line);
	std::cout << line << std::endl;
	assert(line=="<div class=\"grid-container\" style=\" display:grid;\">");
	//assert(htmlMF.Data().sta=="<td style=\" color:green;\">\n\t2.00€\n</td>");
	
	assert(Px<14>::Id=="14px");
	auto css14 = Css<Style<Padding,Px<14>>>();
	std::cout<<"Elements: \n"<<css14<<std::endl;

    return 0;
}
};
