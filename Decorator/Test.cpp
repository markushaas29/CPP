#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "Decorator.hpp"
#include "ElementDecorator.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/HtmlElement.hpp"
#include "../CSV/Style.hpp"
#include "../Quantity/Quantity.hpp"

class DecoratorTest
{
	public:
		int Run()
		{
			std::cout<<"Start Decorator"<<std::endl;
			using QM = Quantity<Mass>;
			using QS = Quantity<Sum>;
			using QV = Quantity<Volume>;
			
			//{"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}}}}
			std::vector<std::string> v = { "Entry", "Abschlagsforderung"};

			FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>> fu = {"Deduction",{{"EqualVisitor", { "IBAN", "DE44600501010008017284"}}}};
			std::unique_ptr<IFactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>> pfu = std::make_unique<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>(fu);
			auto dy = Decorator(Year{2023},fu);
			auto dyp = Decorator(Year{2023},std::move(pfu));
			
			std::cout<<dy<<std::endl;
			assert(fu.Units().size() == 1);
			auto fud = dy();
			assert(fud->Units().size() ==2);

			std::cout<<*fud<<std::endl;

			auto b = B();
			auto qm = QM{5.2};

			assert(b(qm.Data()).Data() == "<b>5.2kg</b>");
			assert(b(qm).Data() == "<b>5.2kg</b>");
			assert(Td()(qm.Data()).Data() == "<td>5.2kg</td>");

//			auto r = Red();
//			auto s = Section();
//			std::cout<<"R"<<r(qm.Data())<<std::endl;
//			std::cout<<"R"<<s(qm.Data())<<std::endl;
//			assert(r(qm.Data()).Data() == "<body style=\"color:red;\">5.2kg</body>");

			auto el = HtmlElement("El","body","TEXT=\"RED\"");
			std::cout<<"EL: "<<el<<std::endl;
			assert(el.Data() == "<body TEXT=\"RED\">El</body>");
//			assert(s(qm).Data() == "<section>5.2kg</section>");
			auto elp = HtmlElement("El","p");
			std::cout<<"EL: "<<el<<std::endl;
			
			auto html = b(qm.Data());
//			auto dd = DecoratorDecider<Red, Green>();
//			auto ddy = dd(true, html);
//			assert(ddy.Data() == "<b style=\"color:red;\">5.2kg</b>");
//			auto ddn = dd(false, html);
//			assert(ddn.Data() == "<b style=\"color:green;\">5.2kg</b>");
			
			auto qsp = QS{5};
			auto qsn = QS{-5};
			auto qsz = QS{0};
			auto html0 = b(qsz.Data());
			auto htmlp = b(qsp.Data());
			auto htmln = b(qsn.Data());
//			auto dds = DecoratorDecider<Green, Red>();
//			auto ddsy = dds(qsp>qsz, htmlp);
//			std::cout<<"EL: "<<ddsy<<std::endl;
//			assert(ddsy.Data() == "<b style=\"color:green;\">5.€</b>");
//			auto ddsn = dds(qsn>qsz, htmln);
//			assert(ddsn.Data() == "<b style=\"color:red;\">-5.€</b>");

			auto br = Style<Border,Red>();
			assert(br.Element()=="Border");

			std::cout<<"End Decorator"<<std::endl;

			return 0;
		}
};

