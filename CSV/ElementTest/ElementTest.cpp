#include <iostream>
#include <regex>
#include <cassert>
#include "ElementRegexTest.hpp"
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../HtmlElement.hpp"
#include "../../Decorator/ElementDecorator.hpp"
#include "../CssStyle.hpp"
#include "../Style.hpp"
#include "../../Common/Color.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"

class ElementTest{
public:
int Run()
{       
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

	auto html = HtmlElement<Entry,Td>(Entry("TEST"));
	assert(html.Data()=="<td style=\" color:black;\">TEST</td>");
	auto htmlN = HtmlElement<Name,Td>(Name("TEST"));
	assert(htmlN.Data()=="<td style=\" color:blue;\">TEST</td>");

	auto htmlQP = HtmlElement<Quantity<Sum>,Td>(Quantity<Sum>{2});
	assert(htmlQP.Data()=="<td style=\" color:green;\">2.00€</td>");
	auto htmlQN = HtmlElement<Quantity<Sum>,Td>(Quantity<Sum>{-2});
	assert(htmlQN.Data()=="<td style=\" color:red;\">-2.00€</td>");

	auto nhtml = HtmlElement<HtmlElement<Name,Td>>(htmlN);
	assert(nhtml.Data()=="<td style=\" color:black;\"><td style=\" color:blue;\">TEST</td></td>");

	std::cout<<"P: "<<nhtml<<std::endl;

    return 0;
}
};
