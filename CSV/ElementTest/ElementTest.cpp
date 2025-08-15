#include <iostream>
#include <regex>
#include <cassert>
#include "ElementRegexTest.hpp"
#include "ElementParserTest.hpp"
#include "HtmlElementTest.hpp"
#include "StyleTest.hpp"
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../HtmlElement.hpp"
#include "../NumericStyle.hpp"
#include "../Styles.hpp"
#include "../CssStyle.hpp"
#include "../Style.hpp"
#include "../../Invoice/Form.hpp"
#include "../../Common/Color.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"
#include "../../Matrix/MatrixHeaders.hpp"
#include "../../Matrix/Matrix.hpp"
#include "../../Matrix/MatrixParsers.hpp"
#include "../../Matrix/MatrixDescriptor.hpp"
#include "../../Matrix/MatrixFormatter.hpp"
#include "../../Matrix/MatrixFormatter.hpp"
#include "../../Matrix/MatrixInitializer.hpp"

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

    assert("100kg"==(std::string)Quantity<Mass>(100));
    assert("100.12€"==(std::string)Quantity<Sum>(100.12));
	//assert(*qp==Quantity<Sum>(100));

	ElementRegexTest er;
	assert(er.Run());

	auto st = Street{"Heinrich-Heine"};
	assert(st==std::string("Heinrich-Heine"));
	auto tw = Town{"Heinrichstadt"};
	assert(tw==std::string("Heinrichstadt"));
	auto pr = Prename{"Heinrich"};
	assert(pr==std::string("Heinrich"));

	auto useE = Entry(se);
	auto useA = Entry("Abschlagsforderung");
	assert(useE==useA);
	useA = Entry("Abschagsforderung");
	assert(useE!=useA);
	useA = Entry("DE6900000000084184");
	assert(useE==useA);

	auto ed1 = Entry{"HeatExtraCosts"};
	assert(ed1==std::string("Heiznebenkosten"));
	ed1 = Entry{"heatextracosts"};
	assert(ed1==std::string("Heiznebenkosten"));
	auto ed2 = Entry{"additional heating costs"};
	assert(ed2==std::string("Heiznebenkosten"));

	auto comdirectE = Entry{"Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET  Buchungstext: Internet Ref. GZ22504152445475/2"};
	auto cEls = comdirectE.GetElements<ComdirectParser>();
	assert(cEls.size() == 4);
	comdirectE = Entry{"Auftraggeber: Bausparkasse Schwäbisch Hall Aktiengesellschaft - Bausparkasse der Vol Buchungstext: 26219070T02 U.A. 02.2025 Ref. 6S2C213N0A823JJ4/70562"};
	cEls = comdirectE.GetElements<ComdirectParser>();
//	assert(cEls.size() == 2);

	using ME2D = MatrixDescriptor<2,std::shared_ptr<IElement>>;
    using M2E = Matrix<2,ME2D>;
	M2E m2ce {
		{std::make_shared<Date>(31,12,2023), std::make_shared<QS>(7.5)},
		{std::make_shared<Entry>("Empfänger: Markus HaasKto/IBAN: DE05660623660009331409 BLZ/BIC: GENODE61DET  Buchungstext: Internet Ref. GZ22504152445475/2"), std::make_shared<Entry>("Auftraggeber: Bausparkasse Schwäbisch Hall Aktiengesellschaft - Bausparkasse der Vol Buchungstext: 26219070T02 U.A. 02.2025 Ref. 6S2C213N0A823JJ4/70562")},
		//{std::make_shared<Entry>("Empfänger: Markus HaasKto/IBAN"), std::make_shared<Entry>("Auftraggeber: Bausparkasse Schwäbisch Hall Aktiengesellschaft - Bausparkasse der Vol Buchungstext: 26219070T02 U.A. 02.2025 Ref. 6S2C213N0A823JJ4/70562")},
	  };
			

	auto m2ceA = m2ce.Apply([&](const auto& e1){ 
			auto ev = IsElementVisitor<Entry>();
			std::vector<std::shared_ptr<IElement>> v;
			if((*e1)->Is(ev))
			{
				auto entry = (*e1)->template As<Entry>(); 
				auto entries = entry.template GetElements<ComdirectParser>();
				while(entries.size()<=3)
					entries.push_back(std::make_shared<Text>("-"));
				v.insert(v.end(),entries.cbegin(), entries.cend());
			}
			return v;
			});
	
	std::cout<<"M:\t"<<m2ceA<<std::endl;

	HtmlElementTest().Run();
	StyleTest().Run();
	ElementParserTest().Run();

	return 0;
}
};
